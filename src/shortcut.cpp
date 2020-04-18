#include "shortcut.h"
#include "keyboarddaemon.h"

#include <boost/tokenizer.hpp>

constexpr std::array<unsigned, 4> additionalModifiers = {0, Mod2Mask, LockMask, Mod2Mask | LockMask};

Shortcut::Shortcut(const std::string &shortcut, KeyboardDaemon &daemon, std::function<void(KeyboardDaemon &)> callback)
    : m_daemon(daemon)
    , m_callback(std::move(callback))
{
    const boost::tokenizer keys(shortcut, boost::char_separator<char>("+-"));

    for (auto it = keys.begin(); it != keys.end(); ++it) {
        if (it.current_token() == "Ctrl") {
            m_modmask |= ControlMask;
        } else if (it.current_token() == "Alt") {
            m_modmask |= Mod1Mask;
        } else if (it.current_token() == "Win") {
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
