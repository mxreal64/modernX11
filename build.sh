#!/usr/bin/env bash
# Copyright (C) 2026 mxreal64
# Licensed under the GPL-3.0 License

set -e 

echo "=== Resolving Linux System Package Flags ==="
PKG_CFLAGS=$(pkg-config --cflags gtk+-3.0 webkit2gtk-4.1 x11)
PKG_LIBS=$(pkg-config --libs gtk+-3.0 webkit2gtk-4.1 x11)

mkdir -p build

echo "=== Step 0: Pre-Compiling 'import std;' Module for GCC Cache ==="
# Seeds your local path gcm.cache directory context instantly so std module loads legally
g++ -std=c++26 -fmodules-ts -fsearch-include-path -c bits/std.cc -o build/std_module.o

echo "=== Step 1: Compiling 'modernX11' Module Interface ==="
g++ -std=c++26 -O3 -march=native -fmodules-ts \
    $PKG_CFLAGS \
    -c src/modernX11.cppm \
    -o build/modernX11.o

echo "=== Step 2: Compiling Main Verification Pipeline ==="
g++ -std=c++26 -O3 -march=native -fmodules-ts \
    $PKG_CFLAGS \
    -c src/main.cpp \
    -o build/main.o

echo "=== Step 3: Executing Machine Binary Code LinkPass ==="
g++ build/std_module.o build/modernX11.o build/main.o \
    $PKG_LIBS \
    -o build/modernX11_demo

echo "=== Compilation Complete! Executing Binary Workflow ==="
./build/modernX11_demo
