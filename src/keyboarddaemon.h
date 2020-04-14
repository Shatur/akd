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
#include "shortcut.h"

#include <memory>
#include <unordered_map>
#include <vector>

class KeyboardDaemon
{
public:
    KeyboardDaemon();

    void setGroups(const std::vector<std::string> &unsplittedGroups);
    void addNextGroupShortcut(const std::string &shortcut);

    [[noreturn]]
    void exec();

    Display &display() const;
    Window root() const;

    void switchToNextGroup();

private:
    // Event handlers
    void applyLayout(const XPropertyEvent &event);
    void removeDestroyedWindow(const XDestroyWindowEvent &event);
    void processShortcuts(const XKeyEvent &event);
    void saveCurrentLayout();

    // Helpers
    [[nodiscard]]
    Window activeWindow();
    void setGroup(const std::vector<std::string> &group);

    std::unordered_map<Window, unsigned char> m_windows;
    std::vector<std::vector<std::string>> m_groups;
    std::vector<Shortcut> m_shortcuts;

    std::unique_ptr<Display, DisplayDeleter> m_display{XOpenDisplay(nullptr)};
    Window m_root;
    int m_xkbEventType;
};

#endif // KEYBOARDDAEMON_H
