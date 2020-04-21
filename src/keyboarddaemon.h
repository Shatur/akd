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
#include "keyboard.h"
#include "layout.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include <X11/XKBlib.h>

class KeyboardSymbols;

class KeyboardDaemon
{
public:
    explicit KeyboardDaemon(int argc, char *argv[]);

    [[noreturn]]
    void processEvents();

    Display &display() const;
    Window root() const;

    void switchToNextLayout();

private:
    // Event handlers
    void applyWindowLayout(const XPropertyEvent &event);
    void removeDestroyedWindow(const XDestroyWindowEvent &event);
    void processShortcuts(const XKeyEvent &event);
    void saveCurrentGroup();

    // Helpers
    void setLayout(size_t layoutIndex);
    void setGroup(unsigned char group);
    void readKeyboardRules();

    void printGroupName(unsigned char group, std::optional<size_t> layoutIndex = std::nullopt) const;
    KeyboardSymbols serverSymbols() const;

    [[nodiscard]]
    Window activeWindow() const;

    const std::unique_ptr<Display, DisplayDeleter> m_display{XkbOpenDisplay(nullptr, &m_xkbEventType, nullptr, nullptr, nullptr, nullptr)};
    const Window m_root = XDefaultRootWindow(m_display.get());
    const Atom m_activeWindowProperty = XInternAtom(m_display.get(), "_NET_ACTIVE_WINDOW", false);
    int m_xkbEventType;

    std::unordered_map<Window, Keyboard> m_windows{{activeWindow(), {}}};
    std::vector<Layout> m_layouts;
    std::vector<Shortcut> m_shortcuts;

    XkbComponentNamesRec m_currentComponents{};
    std::unique_ptr<XkbRF_VarDefsRec, VarDefsDeleter> m_currentVarDefs;
    std::unique_ptr<char [], XlibDeleter> m_currentRulesPath;

    decltype(m_windows)::iterator m_currentWindow = m_windows.begin();
    bool m_ignoreNextLayoutSave = false;
    bool m_printGroups = false;
};

#endif // KEYBOARDDAEMON_H
