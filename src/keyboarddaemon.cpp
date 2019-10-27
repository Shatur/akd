/*
 *  Copyright © 2019 Hennadii Chernyshchyk <genaloner@gmail.com>
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
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

constexpr std::string_view activeWindowPropertyName = "_NET_ACTIVE_WINDOW";

KeyboardDaemon::KeyboardDaemon()
{
    if (!m_display)
        throw std::runtime_error("Unable to open display");

    // Listen for events
    if (!XSelectInput(m_display.get(), m_root, PropertyChangeMask | SubstructureNotifyMask))
        throw std::runtime_error("Unable to select X11 inputs");

    if (!XkbQueryExtension(m_display.get(), nullptr, &m_xkbEventType, nullptr, nullptr, nullptr))
        throw std::runtime_error("Unable to query XKB extension");

    if (!XkbSelectEvents(m_display.get(), XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask))
        throw std::runtime_error("Unable select XKB events");
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
    if (!XkbLockGroup(m_display.get(), XkbUseCoreKbd, it != m_windows.end() ? it->second : 0))
        throw std::runtime_error("Unable to change layout");
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

    if (XGetWindowProperty(m_display.get(), m_root, activeWindowProperty, 0, 1, false, AnyPropertyType,
                           &type, &format, &size, &remainSize, &bytes)) {
        throw std::runtime_error("Unable to get active window");
    }

    return *reinterpret_cast<Window *>(bytes);
}
