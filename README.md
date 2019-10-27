# Advanced Keyboard Daemon*

[![GitHub (pre-)release](https://img.shields.io/github/release/Shatur95/akd/all.svg)](https://github.com/Shatur95/akd/releases)
[![Codacy grade](https://img.shields.io/codacy/grade/6a80ccb47c11497c8e89efa52d5714b7.svg)](https://app.codacy.com/project/Shatur95/akd/dashboard)

**Advanced Keyboard Daemon** is a keyboard daemon written in C++17 that allows you to use different layouts for each window. Just run it and it will do its job.

## Content

-   [Features](#features)
-   [Dependencies](#dependencies)
-   [Installation](#installation)

## Features

-   Use different keyboard layout for each window.

## Dependencies

**Arch Linux:** cmake libx11

## Installation

### Pacman-based (Arch Linux, Manjaro, Chakra etc.)

You can install [akd](https://aur.archlinux.org/packages/akd "A Keyboard userspace daemon") from AUR.

### Manual building

You can build **Advanced Keyboard Daemon** by using the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

You will then get a binary named `akd`.

<sub>*Currently is not advanced</sub>
