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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "shortcut.h"

#include "keyboarddaemon.h"

#include <boost/tokenizer.hpp>

constexpr std::array<unsigned, 4> additionalModifiers = {0, Mod2Mask, LockMask, Mod2Mask | LockMask};

Shortcut::Shortcut(const std::string &shortcut, KeyboardDaemon &daemon, std::function<void(KeyboardDaemon &)> callback)
    : m_daemon(daemon)
    , m_callback(std::move(callback))
{
    const boost::tokenizer keys(shortcut, boost::char_separator<char>("+"));

    for (auto it = keys.begin(); it != keys.end(); ++it) {
        if (it.current_token() == "Ctrl") {
            m_modmask |= ControlMask;
        } else if (it.current_token() == "Alt") {
            m_modmask |= Mod1Mask;
        } else if (it.current_token() == "Meta") {
            m_modmask |= Mod4Mask;
        } else if (it.current_token() == "Shift") {
            m_modmask |= ShiftMask;
        } else {
            if (m_keycode)
                throw std::logic_error("You can't specify more then one key in shortcut: " + shortcut);
            const KeySym keySum = XStringToKeysym(it->data());
            if (keySum == NoSymbol)
                throw std::logic_error("Unable to get keysum from " + it.current_token());
            m_keycode = XKeysymToKeycode(&daemon.display(), keySum);
        }
    }

    if (!m_keycode)
        throw std::logic_error("You cannot bind shortcut without keys: " + shortcut);

    // Need to bind all combinations with CapsLock and ScrollLock to make it work
    for (unsigned specialModifier : additionalModifiers) {
        if (!XGrabKey(&daemon.display(), m_keycode.value(), m_modmask | specialModifier, daemon.root(), true, GrabModeAsync, GrabModeAsync))
            std::logic_error("Unable to register shortcut " + shortcut);
    }
}

void Shortcut::processEvent(const XKeyEvent &event) const
{
    bool currentModifiers = std::any_of(additionalModifiers.begin(), additionalModifiers.end(), [this, &event](unsigned additionalModifier) {
        return event.state == (m_modmask | additionalModifier);
    });

    if (currentModifiers && event.keycode == m_keycode)
        m_callback(m_daemon);
}
