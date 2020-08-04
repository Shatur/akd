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

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <functional>
#include <string>

#include <X11/Xlib.h>

class KeyboardDaemon;

class Shortcut
{
public:
    Shortcut(const std::string &shortcut, KeyboardDaemon &daemon, std::function<void(KeyboardDaemon &)> callback);

    void processEvent(const XKeyEvent &event) const;

private:
    unsigned m_modmask = 0;
    std::optional<KeyCode> m_keycode;

    KeyboardDaemon &m_daemon;
    std::function<void(KeyboardDaemon &)> m_callback;
};

#endif // SHORTCUT_H
