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

#ifndef KEYBOARDDAEMON_H
#define KEYBOARDDAEMON_H

#include "x11deleters.h"

#include <memory>
#include <unordered_map>
#include <vector>

class KeyboardDaemon
{
public:
    KeyboardDaemon();

    void setGroups(const std::vector<std::string> &unsplittedGroups);

    [[noreturn]]
    void exec();

private:
    // Event handlers
    void switchLayout(XPropertyEvent *event);
    void removeDestroyedWindow(XDestroyWindowEvent *event);
    void saveCurrentLayout();
    void setGroup(const std::vector<std::string> &group);

    // Helpers
    [[nodiscard]]
    Window activeWindow();

    std::unique_ptr<Display, DisplayDeleter> m_display{XOpenDisplay(nullptr)}; // Initialize connection to X11
    std::unordered_map<Window, unsigned char> m_windows;
    std::vector<std::vector<std::string>> m_groups;

    Window m_root;
    int m_xkbEventType;
};

#endif // KEYBOARDDAEMON_H
