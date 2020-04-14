/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Crow Translate is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "keyboarddaemon.h"
#include "keyboardsymbolsparser.h"

#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <X11/XKBlib.h>

namespace x3 = boost::spirit::x3;

constexpr std::string_view activeWindowPropertyName = "_NET_ACTIVE_WINDOW";

KeyboardDaemon::KeyboardDaemon()
{
    if (!m_display)
        throw std::runtime_error("Unable to open display");

    m_root = XDefaultRootWindow(m_display.get());

    // Listen for events
    XSelectInput(m_display.get(), m_root, PropertyChangeMask | SubstructureNotifyMask);
    XkbQueryExtension(m_display.get(), nullptr, &m_xkbEventType, nullptr, nullptr, nullptr);
    XkbSelectEvents(m_display.get(), XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask);
}

void KeyboardDaemon::setGroups(const std::vector<std::string> &unsplittedGroups)
{
    for (const std::string &group : unsplittedGroups) {
        std::vector<std::string> splittedGroup;
        boost::split(splittedGroup, group, boost::is_any_of(","));
        m_groups.push_back(std::move(splittedGroup));
    }

    if (!m_groups.empty())
        setGroup(m_groups.front());
}

void KeyboardDaemon::addNextGroupShortcut(const std::string &shortcut)
{
    m_shortcuts.emplace_back(shortcut, *this, &KeyboardDaemon::switchToNextGroup);
}

void KeyboardDaemon::exec()
{
    while (true) {
        XEvent event;
        XNextEvent(m_display.get(), &event);

        if (event.type == DestroyNotify) {
            removeDestroyedWindow(reinterpret_cast<XDestroyWindowEvent *>(&event));
        } else if (event.type == PropertyNotify) {
            // Current window changed
            switchLayout(reinterpret_cast<XPropertyEvent *>(&event));  
        } else if (event.type == KeyPress) {
            for (const Shortcut &shortcut : m_shortcuts)
                shortcut.processEvent(event);
        } else if (event.type == m_xkbEventType) {
            // Current layout changed
            saveCurrentLayout();
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

void KeyboardDaemon::switchToNextGroup()
{
    std::cout << "Currently not implemented!" << std::endl;
}

void KeyboardDaemon::removeDestroyedWindow(XDestroyWindowEvent *event)
{
    m_windows.erase(event->window);
}

void KeyboardDaemon::switchLayout(XPropertyEvent *event)
{
    if (event->state != PropertyNewValue)
        return;

    const std::unique_ptr<char [], XlibDeleter> propertyEventName(XGetAtomName(m_display.get(), event->atom));
    if (propertyEventName.get() != activeWindowPropertyName)
        return;

    const auto it = m_windows.find(activeWindow());
    XkbLockGroup(m_display.get(), XkbUseCoreKbd, it != m_windows.end() ? it->second : 0);
}

void KeyboardDaemon::saveCurrentLayout()
{
    XkbStateRec state;
    XkbGetState(m_display.get(), XkbUseCoreKbd, &state);

    m_windows.insert_or_assign(activeWindow(), state.group);
}

Window KeyboardDaemon::activeWindow()
{
    const Atom activeWindowProperty = XInternAtom(m_display.get(), activeWindowPropertyName.data(), false);
    Atom type;
    int format;
    unsigned long size;
    unsigned long remainSize;
    unsigned char *bytes;

    XGetWindowProperty(m_display.get(), m_root, activeWindowProperty, 0, 1, false, AnyPropertyType,
                       &type, &format, &size, &remainSize, &bytes);

    std::unique_ptr<unsigned char [], XlibDeleter> cleaner(bytes);

    return *reinterpret_cast<Window *>(bytes);
}

void KeyboardDaemon::setGroup(const std::vector<std::string> &group)
{
    // Read info from X11
    const std::unique_ptr<XkbDescRec, XlibDeleter> currentDesc(XkbGetKeyboardByName(m_display.get(), XkbUseCoreKbd, nullptr, XkbGBN_ServerSymbolsMask | XkbGBN_KeyNamesMask, 0, false));
    const std::unique_ptr<char [], XlibDeleter> currentSymbols(XGetAtomName(m_display.get(), currentDesc->names->symbols));

    // Parse to structure
    KeyboardSymbols parsedSymbols;
    x3::phrase_parse(currentSymbols.get(), currentSymbols.get() + strlen(currentSymbols.get()), KeyboardSymbolsParser::symbolsRule, x3::space, parsedSymbols);

    // Replace layouts with specified and generate new symbols string
    parsedSymbols.layouts = group;
    std::string newSymbols = parsedSymbols.x11String();

    // Send it back to X11
    XkbComponentNamesRec componentNames = {nullptr, nullptr, nullptr, nullptr, newSymbols.data(), nullptr};
    const std::unique_ptr<XkbDescRec, XlibDeleter> newDesc(XkbGetKeyboardByName(m_display.get(), XkbUseCoreKbd, &componentNames, XkbGBN_ClientSymbolsMask | XkbGBN_KeyNamesMask, 0, false));
    if (!newDesc)
        throw std::logic_error("Unable to build keyboard description with the following symbols: " + newSymbols);

    if (!XkbSetMap(m_display.get(), XkbKeySymsMask, newDesc.get()))
        throw std::logic_error("Unable to set the following symbols: " + newSymbols);
}
