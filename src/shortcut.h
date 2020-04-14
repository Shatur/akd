#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <string>
#include <functional>

class _XDisplay;
class KeyboardDaemon;
union _XEvent;

using KeyCode = unsigned char;
using Display = _XDisplay;
using XEvent = _XEvent;

class Shortcut
{
public:
    Shortcut(const std::string &shortcut, KeyboardDaemon &daemon, std::function<void(KeyboardDaemon &)> callback);

    void processEvent(const XEvent &event) const;

private:
    unsigned m_modmask = 0;
    std::optional<KeyCode> m_keycode;

    KeyboardDaemon &m_daemon;
    std::function<void(KeyboardDaemon &)> m_callback;
};

#endif // SHORTCUT_H
