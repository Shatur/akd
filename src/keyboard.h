#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstddef>

struct Keyboard
{
    unsigned char group = 0;
    size_t layoutIndex = 0;
};

#endif // KEYBOARD_H
