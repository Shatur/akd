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

#include <iostream>

#include <X11/XKBlib.h>

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

void KeyboardDaemon::setGroups(std::vector<std::string> groups)
{
    m_groups = std::move(groups);

    std::string keycodes = "evdev+aliases(qwerty)";
    std::string types = "complete";
    std::string compat = "complete";
    std::string symbols = "pc+us+ru:2+inet(evdev)+capslock(grouplock)+group(alt_shift_toggle)+terminate(ctrl_alt_bksp)";
    std::string geometry = "pc(pc101)";
    XkbComponentNamesRec componentNames = {nullptr, keycodes.data(), types.data(), compat.data(), symbols.data(), geometry.data()};
    const std::unique_ptr<XkbDescRec, XlibDeleter> clientDesc(XkbGetKeyboardByName(m_display.get(), XkbUseCoreKbd, &componentNames, XkbGBN_AllComponentsMask, XkbGBN_AllComponentsMask, false));

    std:: cout << XkbSetMap(m_display.get(), XkbKeySymsMask, clientDesc.get()) << std::endl;
    throw std::logic_error("Done!");

    // Here need to apply first specified group
//    const std::unique_ptr<XkbDescRec, XlibDeleter> serverDesc(XkbGetKeyboardByName(m_display.get(), XkbUseCoreKbd, nullptr, XkbGBN_ServerSymbolsMask, XkbGBN_ServerSymbolsMask, false));
//    const std::unique_ptr<char [], XlibDeleter> symbols(XGetAtomName(m_display.get(), serverDesc->names->symbols));

//    std::cout << symbols.get() << std::endl;
//    std::string parsedSymbols;
//    std::replace_copy(symbols.get(), symbols.get() + strlen(symbols.get()), std::back_inserter(parsedSymbols), '_', '+');
//    std::cout << parsedSymbols << std::endl;
//    const std::unique_ptr<char [], XlibDeleter> symbols(XGetAtomName(m_display.get(), clientDesc->names->symbols));
//    std::cout << clientDesc.get() << std::endl;

//    XkbMapChangesRec changes;
//    changes.changed = XkbVirtualModMapMask;
//    const std::unique_ptr<char [], XlibDeleter> test(XGetAtomName(m_display.get(), clientDesc->names->symbols));
//    std::cout << test .get() << std::endl;
//    XkbChangeMap(m_display.get(), )
//    std::replace_copy(symbols.get(), strlen(symbols.get()), std::back_inserter(newSymbols), '_', '+');
//        std::vector<std::string> components;
//        boost::split(components, group, boost::is_any_of(","));
//        if (components.size() != 5)
//            throw std::logic_error(str(boost::format("Specified group %1% have only %2% components (5 expected)") % group % components.size()));

//        XkbComponentNamesRec componentNames;
//        componentNames.keymap = nullptr;
//        componentNames.keycodes = components[0].data();
//        componentNames.types = components[1].data();
//        componentNames.compat = components[2].data();
//        componentNames.symbols = components[3].data();
//        componentNames.geometry = components[4].data();

//        m_groups.emplace_back(XkbGetKeyboardByName(m_display.get(), XkbUseCoreKbd, &componentNames, XkbGBN_AllComponentsMask, XkbGBN_AllComponentsMask, false));

    // Apply first group
//    XkbSetMap(m_display.get(), XkbGBN_AllComponentsMask & (~XkbGBN_GeometryMask), m_groups.front().get());
}

void KeyboardDaemon::exec()
{
    while (true) {
        XEvent event;
        XNextEvent(m_display.get(), &event);

        if (event.type == DestroyNotify)
            removeDestroyedWindow(reinterpret_cast<XDestroyWindowEvent *>(&event));
        else if (event.type == PropertyNotify) // Current window changed
            switchLayout(reinterpret_cast<XPropertyEvent *>(&event));
        else if (event.type == m_xkbEventType) // Current layout changed
            saveCurrentLayout();
    }
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
    m_windows[activeWindow()] = state.group;
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
