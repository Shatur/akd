/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Advanced Keyboard Daemon is free software; you can redistribute it and/or modify
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

#include "keyboard.h"
#include "layout.h"
#include "shortcut.h"
#include "x11deleters.h"

#include <memory>
#include <unordered_map>

#include <X11/XKBlib.h>

class KeyboardSymbols;
class Parameters;

class KeyboardDaemon
{
public:
    explicit KeyboardDaemon(const Parameters &parameters);

    [[nodiscard]] bool needProcessEvents() const;
    [[noreturn]] void processEvents();

    [[nodiscard]] Display &display() const;
    [[nodiscard]] Window root() const;

    void switchToNextLayout();

private:
    // Event handlers
    void applyWindowLayout(const XPropertyEvent &event);
    void removeDestroyedWindow(const XDestroyWindowEvent &event);
    void processShortcuts(const XKeyEvent &event);
    void saveCurrentGroup(const XkbStateNotifyEvent &event);

    // Helpers
    void setLayout(size_t layoutIndex);
    void setGroup(unsigned char group);

    void loadParameters(const Parameters &parameters);
    void saveCurrentGroup();

    void printCurrentGroup() const;
    void printGroupFromKeyboardRules(unsigned char group) const;
    void printGroupIfDifferent(unsigned char newGroup, size_t newLayoutIndex) const;
    [[nodiscard]] KeyboardSymbols serverSymbols() const;
    [[nodiscard]] Window activeWindow() const;
    [[nodiscard]] unsigned char currentGroup() const;

    const std::unique_ptr<Display, DisplayDeleter> m_display{XkbOpenDisplay(nullptr, &m_xkbEventType, nullptr, nullptr, nullptr, nullptr)};
    Window m_root;
    Atom m_activeWindowProperty;
    int m_xkbEventType;

    std::unordered_map<Window, Keyboard> m_windows;
    std::vector<Layout> m_layouts;
    std::vector<Shortcut> m_shortcuts;

    decltype(m_windows)::iterator m_currentWindow;
    std::optional<unsigned char> m_defaultGroup;
    bool m_ignoreNextGroupSave = false;
    bool m_needProcessEvents;
    bool m_useDifferentGroups;
    bool m_useDifferentLayouts;
    bool m_printGroups;
};

#endif // KEYBOARDDAEMON_H
