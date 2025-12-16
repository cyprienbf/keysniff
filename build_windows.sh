#!/bin/bash

# ===========================
#  Compile Script for Windows
# ===========================

# Configuration
BUILD_DIR="bin_windows"
BINARY_NAME="keysniff.exe"
COMPILER="clang"
CROSS_FILE="cross_clang.txt"

set -e

export CC=$COMPILER

if [ ! -f "subprojects/curl.wrap" ]; then
    mkdir -p subprojects
    meson wrap install curl
fi

# Find .c and .h files recursively in src/ and include/ and format them
find src include -name "*.[ch]" | xargs clang-format -i -style=file

# Setup / Configure Meson
if [ ! -d "$BUILD_DIR" ]; then
    meson setup "$BUILD_DIR" \
        --cross-file "$CROSS_FILE" \
        --buildtype=release \
        --force-fallback-for=libcurl
        -Dcurl:tls=schannel \
        -Dcurl:tool=disabled
else
    meson setup --reconfigure "$BUILD_DIR" \
        --cross-file "$CROSS_FILE" \
        --buildtype=release \
        --force-fallback-for=libcurl
        -Dcurl:tls=schannel \
        -Dcurl:tool=disabled
fi

# Compile
meson compile -C "$BUILD_DIR"

# Display binary info
file "${BUILD_DIR}/${BINARY_NAME}"