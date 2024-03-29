/*
 *  Copyright © 2019-2021 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Advanced Keyboard Daemon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Advanced Keyboard Daemon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Advanced Keyboard Daemon. If not, see <https://www.gnu.org/licenses/>.
 */

#include "keyboarddaemon.h"

#include "keyboardsymbols.h"
#include "parameters.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/tokenizer.hpp>

#include <iostream>

#include <X11/extensions/XKBrules.h>

KeyboardDaemon::KeyboardDaemon(const Parameters &parameters)
{
    if (!m_display)
        throw std::logic_error("Unable to connect to X server");

    if (parameters.isPrintCurrentGroup()) {
        printGroupFromKeyboardRules(currentGroup());
        m_needProcessEvents = false;
        return;
    }

    if (parameters.isPrintCurrentGroupIndex()) {
        std::cout << static_cast<int>(currentGroup()) << std::endl;
        m_needProcessEvents = false;
        return;
    }

    if (std::optional<unsigned char> group = parameters.groupToSet(); group) {
        setGroup(group.value());
        m_needProcessEvents = false;
        return;
    }

    if (parameters.isSwitchToNextGroup()) {
        setGroup(currentGroup() + 1);
        m_needProcessEvents = false;
        return;
    }

    m_root = XDefaultRootWindow(m_display.get());
    m_activeWindowProperty = XInternAtom(m_display.get(), "_NET_ACTIVE_WINDOW", false);
    m_windows.emplace(activeWindow(), Keyboard());
    m_currentWindow = m_windows.begin();

    loadParameters(parameters);

    XkbSelectEventDetails(m_display.get(), XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);
    if (m_useDifferentGroups || m_useDifferentLayouts)
        XSelectInput(m_display.get(), m_root, PropertyChangeMask | SubstructureNotifyMask); // Listen for current window change events

    saveCurrentGroup();
    m_needProcessEvents = true;
}

bool KeyboardDaemon::needProcessEvents() const
{
    return m_needProcessEvents;
}

void KeyboardDaemon::processEvents()
{
    XkbEvent event;

    while (true) {
        XNextEvent(m_display.get(), &event.core);

        switch (event.type) {
        case DestroyNotify:
            removeDestroyedWindow(event.core.xdestroywindow);
            break;
        case PropertyNotify:
            applyWindowLayout(event.core.xproperty);
            break;
        case KeyPress:
            processShortcuts(event.core.xkey);
            break;
        default:
            if (event.type == m_xkbEventType)
                saveCurrentGroup(event.state);
        }
    }
}

Display &KeyboardDaemon::display() const
{
    return *m_display;
}

Window KeyboardDaemon::root() const
{
    return m_root;
}

void KeyboardDaemon::switchToNextLayout()
{
    size_t layoutIndex = m_currentWindow->second.layoutIndex + 1;
    if (layoutIndex >= m_layouts.size())
        layoutIndex = 0;

    setLayout(layoutIndex);

    if (m_defaultGroup && m_currentWindow->second.group != m_defaultGroup.value()) {
        setGroup(m_defaultGroup.value());
        printGroupIfDifferent(m_defaultGroup.value(), layoutIndex);

        m_currentWindow->second.group = m_defaultGroup.value();
    } else {
        printGroupIfDifferent(m_currentWindow->second.group, layoutIndex);
    }

    m_currentWindow->second.layoutIndex = layoutIndex;
}

void KeyboardDaemon::applyWindowLayout(const XPropertyEvent &event)
{
    if (event.state != PropertyNewValue || event.atom != m_activeWindowProperty)
        return;

    const auto [newWindow, inserted] = m_windows.try_emplace(activeWindow());
    if (m_useDifferentLayouts) {
        if (newWindow->second.layoutIndex != m_currentWindow->second.layoutIndex)
            setLayout(newWindow->second.layoutIndex);
    } else {
        newWindow->second.layoutIndex = m_currentWindow->second.layoutIndex;
    }

    if (m_useDifferentGroups) {
        if (newWindow->second.group != m_currentWindow->second.group)
            setGroup(newWindow->second.group);
    } else {
        newWindow->second.group = m_currentWindow->second.group;
    }

    printGroupIfDifferent(newWindow->second.group, newWindow->second.layoutIndex);
    m_currentWindow = newWindow;
}

void KeyboardDaemon::removeDestroyedWindow(const XDestroyWindowEvent &event)
{
    m_windows.erase(event.window);
}

void KeyboardDaemon::processShortcuts(const XKeyEvent &event)
{
    for (const Shortcut &shortcut : m_shortcuts)
        shortcut.processEvent(event);
}

void KeyboardDaemon::saveCurrentGroup(const XkbStateNotifyEvent &event)
{
    if (m_ignoreNextGroupSave) {
        m_ignoreNextGroupSave = false;
        return;
    }

    m_currentWindow->second.group = event.group;
    printCurrentGroup();
}

void KeyboardDaemon::setLayout(size_t layoutIndex)
{
    m_layouts[layoutIndex].apply();
}

void KeyboardDaemon::setGroup(unsigned char group)
{
    if (!XkbLockGroup(m_display.get(), XkbUseCoreKbd, group))
        throw std::logic_error("Unable to switch group to " + std::to_string(group));

    // This will produce XkbStateNotifyEvent event, ignore it
    m_ignoreNextGroupSave = true;
}

void KeyboardDaemon::loadParameters(const Parameters &parameters)
{
    m_defaultGroup = parameters.defaultGroup();
    m_useDifferentGroups = parameters.isUseDifferentGroups();
    m_useDifferentLayouts = parameters.useDifferentLayouts();
    m_printGroups = parameters.isPrintGroups();

    const KeyboardSymbols symbols = KeyboardSymbols::currentSymbols(*m_display);
    if (std::optional<std::vector<std::string>> layouts = parameters.layouts(); layouts) {
        for (std::string &layout : layouts.value())
            m_layouts.emplace_back(*m_display, std::move(layout), symbols.options);
        if (!parameters.isSkipRules())
            Layout::saveKeyboardRules(*m_display);
        setLayout(0);
    } else {
        m_layouts.emplace_back(*m_display, boost::join(symbols.groups, ","));
    }

    if (const std::optional<std::string> nextLayout = parameters.nextLayoutShortcut(); nextLayout)
        m_shortcuts.emplace_back(nextLayout.value(), *this, &KeyboardDaemon::switchToNextLayout);
}

void KeyboardDaemon::saveCurrentGroup()
{
    const unsigned char group = currentGroup();

    m_currentWindow->second.group = group;
    printCurrentGroup();
}

void KeyboardDaemon::printCurrentGroup() const
{
    if (!m_printGroups)
        return;

    std::cout << m_layouts[m_currentWindow->second.layoutIndex].groupName(m_currentWindow->second.group) << std::endl;
}

void KeyboardDaemon::printGroupFromKeyboardRules(unsigned char group) const
{
    std::unique_ptr<XkbRF_VarDefsRec, VarDefsDeleter> currentVarDefs(new XkbRF_VarDefsRec);
    if (!XkbRF_GetNamesProp(m_display.get(), nullptr, currentVarDefs.get()))
        throw std::logic_error("Unable to get keyboard rules");

    boost::tokenizer layoutTokenizer(std::string_view(currentVarDefs->layout), boost::char_separator(","));
    auto currentGroupName = layoutTokenizer.begin();
    std::advance(currentGroupName, group);

    std::cout << currentGroupName.current_token() << '\n';
}

void KeyboardDaemon::printGroupIfDifferent(unsigned char newGroup, size_t newLayoutIndex) const
{
    if (!m_printGroups)
        return;

    // Compare groups lexically to check if groups different
    const std::string_view newGroupName = m_layouts[newLayoutIndex].groupName(newGroup);
    const std::string_view currentGroupName = m_layouts[m_currentWindow->second.layoutIndex].groupName(m_currentWindow->second.group);
    if (newGroupName != currentGroupName)
        std::cout << newGroupName << std::endl;
}

Window KeyboardDaemon::activeWindow() const
{
    Atom type;
    int format;
    unsigned long size;
    unsigned long remainSize;
    unsigned char *bytes;

    const int result = XGetWindowProperty(m_display.get(), m_root, m_activeWindowProperty, 0, 1, false, AnyPropertyType,
                                          &type, &format, &size, &remainSize, &bytes);

    if (result != Success)
        throw std::logic_error("Unable to get active window property");

    // There is no active window
    if (type == None)
        return m_root;

    Window window = *reinterpret_cast<Window *>(bytes);
    XFree(bytes);

    return window;
}

unsigned char KeyboardDaemon::currentGroup() const
{
    XkbStateRec state;
    XkbGetState(m_display.get(), XkbUseCoreKbd, &state);
    return state.group;
}
