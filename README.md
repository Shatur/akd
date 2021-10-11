# Advanced Keyboard Daemon

[![GitHub (pre-)release](https://img.shields.io/github/release/Shatur95/akd/all.svg)](https://github.com/Shatur95/akd/releases)

**Advanced Keyboard Daemon** is a small and efficient keyboard daemon for X11 written in C++17.

## Content

- [Advanced Keyboard Daemon](#advanced-keyboard-daemon)
  - [Content](#content)
  - [Features](#features)
  - [Dependencies](#dependencies)
  - [Installation](#installation)
    - [Arch Linux, Manjaro, Chakra etc.](#arch-linux-manjaro-chakra-etc)
    - [Manual building](#manual-building)

## Features

- Remember different keyboard layouts and groups for each window.
- Switch layouts by shortcut.
- Layout configuration.
- Print group to `stdout` on change (or just once and exit).
- Low memory consumption (~350 KB).

Read `man akd` for more information or pass `-h` / `--help` as argument.

**Note:** We use X11 naming convention: group is the keyboard language, layout is the set of languages.

## Dependencies

**Arch Linux:** boost cmake libx11

## Installation

### Arch Linux, Manjaro, Chakra etc.

:package: [Stable version in AUR](https://aur.archlinux.org/packages/akd)

```bash
git clone https://aur.archlinux.org/akd.git
cd akd
makepkg -si
```

### Manual building

You can build **Advanced Keyboard Daemon** by using the following commands:

```bash
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build .
```

You will then get a binary named `akd`.

## Tips

### `i3bar` keyboard layout indicator

You can use example scripts in `contrib/` directory to add a keyboard layout indicator to your `i3bar`, if you use `i3status` as the status generator. For more information on how they work refer to [i3status docs](https://i3wm.org/docs/i3status.html).

Also, the scripts should be easily adaptable for other text-based status generators.
