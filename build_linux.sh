#!/bin/bash

# =========================
#  Compile Script for Linux
# =========================

# Configuration
BUILD_DIR="bin_linux"
BINARY_NAME="keysniff"
COMPILER="clang"

set -e

export CC=$COMPILER

# Find .c and .h files recursively in src/ and include/ and format them
find src include -name "*.[ch]" | xargs clang-format -i -style=file

# Setup / Configure Meson
if [ ! -d "$BUILD_DIR" ]; then
    meson setup "$BUILD_DIR" --buildtype=release
else
    meson setup --reconfigure "$BUILD_DIR" --buildtype=release
fi

# Compile
meson compile -C "$BUILD_DIR"

# Display binary info
file "${BUILD_DIR}/${BINARY_NAME}"
