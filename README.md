[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
# 2D_Engine_C

## Description
A multi-platform 2D_Engine made in C.

## Summary

 1. [Licenses](#licences)
 2. [Building](#building)

## Licences
- This program is open-source : you can redistribute and/or modify it under the term of the **GNU GPLv3**.
  Copyright (C) 2025 - 2026 Ulysse Cura. See [LICENSE](LICENSE.txt) or [gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html).

## Building

### Setup

### Linux

The only tools needed are :
 - gcc
 - make
 - pkg-config

With the librairies (tested with version) :
 - sdl3     version >= 3.2.1O
 - libpng   version >= 1.6.48
 - json-c   version >= 0.18

On Debian, the command to install dependencies is :
```
sudo apt install gcc make pkg-config libsdl3-dev libpng-dev libjson-c-dev
```

On Nix, the shell.nix file manage the dependencies for you, but it might not be up to date, as support for Nix is not a priority.

### Windows

To build on windows you will need MSYS2 or Cygwin.
For the moment only MSYS2 is officialy supported.

The only tools needed are :
 - x86_64-w64-mingw32-gcc
 - make
 - x86_64-w64-mingw32-pkg-config

With the librairies (tested with version) :
 - sdl3     version >= 3.4.10
 - libpng   version >= 1.6.58
 - json-c   version >= 0.18

On MSYS2, the command to install dependencies is :
```
pacman -S mingw-w64-ucrt-x86_64-gcc make mingw-w64-ucrt-x86_64-pkg-config mingw-w64-ucrt-x86_64-sdl3 mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-json-c
```

### Cross compiling

For the moment cross compiling from linux to windows is the only supported cross compiling option.
To do that you will need the following tools :
 - x86_64-w64-mingw32-gcc
 - make
 - x86_64-w64-mingw32-pkg-config

With the librairies (tested with version) :
 - sdl3     version >= 3.4.10 (mingw devel)
 - libpng   version >= 1.6.58
 - json-c   version >= 0.18

On Debian the command to install dependencies is :
```
sudo apt install mingw-w64 mingw-w64-tools mingw-w64-ucrt64-dev make libpng-dev libjson-c-dev
```

For the sdl3 mingw devel package you will need to install it manually.
See [README-windows](https://wiki.libsdl.org/SDL3/README-windows).
Or [releases](https://github.com/libsdl-org/SDL/releases).

### Commands

The [configure](configure) script automaticaly test compilers and libs versions for your target host.

See `./configure --help` for usage.

The aim is to support linux, windows and the fxcg50 calculator.
However the option `--target-host` only officialy supports `linux` and `windows` but `fxcg50` is also an accepted option.
