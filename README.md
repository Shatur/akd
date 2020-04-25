# Advanced Keyboard Daemon

[![GitHub (pre-)release](https://img.shields.io/github/release/Shatur95/akd/all.svg)](https://github.com/Shatur95/akd/releases)
[![Codacy grade](https://img.shields.io/codacy/grade/6a80ccb47c11497c8e89efa52d5714b7.svg)](https://app.codacy.com/project/Shatur95/akd/dashboard)

**Advanced Keyboard Daemon** is a small and efficient keyboard daemon for X11 written in C++17.

## Content

- [Features](#features)
- [Configuration](#configuration)
- [Dependencies](#dependencies)
- [Installation](#installation)

## Features

- Low memory consumption (~350 KB).
- Layout configuration.
- Remember different keyboard layouts and groups for each window.
- Print layout to `stdout` on change.

## Configuration

### CLI

**Usage:** `akd [options]`

**Commands**

| Option                                             | Description                       |
| -------------------------------------------------- | --------------------------------- |
| -h [ --help ]                                      | Print usage information and exit. |
| -v [ --version ]                                   | Print version number and exit.    |
| -s [ --settings ] path (="~/.config/akd/akd.conf") | Path to settings file.            |

**Configuration**

| Option                             | Description                                                                              |
| ---------------------------------- | ---------------------------------------------------------------------------------------- |
| -g [ --general.different-groups ]  | Use different groups for each window.                                                    |
| -a [ --general.different-layout ]  | Use different layouts for each window.                                                   |
| -p [ --general.print-groups ]      | Print switched languages in stdout.                                                      |
| -r [ --general.skip-rules ]        | Do not update keyboard rules, useful if you use only this program to work with keyboard. |
| -l [ --general.layouts ] arg       | Languages separated by ','. Can be specified several times to define several layouts.    |
| -n [ --shortcuts.next-layout ] arg | Switch to next layout.                                                                   |

**Note:** Shortcuts is a strings with keys separated by `+`. For example: `-n Meta+Shift+L`.

### Configuration file

To avoid typing the same options every time you can use configuration file (from `--settings` option) in combination with CLI parameters (CLI takes precedence over the configuration file). Settings uses [INI file](https://en.wikipedia.org/wiki/INI_file) syntax and contains the same options as in **configuration** table. `general.layouts` defines `[general]` section and `layouts` value. For example:

```ini
[general]
print-groups=true
layouts=en,ru en,ua

[shortcuts]
next-layout=Ctrl+Alt+F
```

is equivalent to:

```bash
akd -p -l en,ru en,ua -n Ctrl+Alt+F
```

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
