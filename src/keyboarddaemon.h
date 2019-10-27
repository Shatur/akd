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

#ifndef KEYBOARDDAEMON_H
#define KEYBOARDDAEMON_H

#include "x11deleters.h"

#include <memory>
#include <unordered_map>

class KeyboardDaemon
{
public:
    KeyboardDaemon();

    [[noreturn]]
    void exec();

private:
    // Event handlers
    void switchLayout(XPropertyEvent *event);
    void removeDestroyedWindow(XDestroyWindowEvent *event);
    void saveCurrentLayout();

    // Helpers
    [[nodiscard]]
    Window activeWindow();

    // Initialize connection to X11
    std::unique_ptr<Display, DisplayDeleter> m_display{XOpenDisplay(nullptr)};
    Window m_root = XDefaultRootWindow(m_display.get());

    std::unordered_map<Window, unsigned char> m_windows;
    int m_xkbEventType;
};

#endif // KEYBOARDDAEMON_H
