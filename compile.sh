#!/bin/bash

# ==========================================
#  Compile Script for Keysniff (Clang/amd64)
# ==========================================

# 1. Configuration
BUILD_DIR="builddir"
BINARY_NAME="keysniff"
COMPILER="clang"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Exit if a command exits with a non-zero status
set -e

echo -e "${YELLOW}[*] Checking environment...${NC}"

# Check if Clang is installed
if ! command -v $COMPILER &> /dev/null; then
    echo -e "${RED}[!] Error: $COMPILER is not installed.${NC}"
    exit 1
fi

# Set the compiler environment variable
export CC=$COMPILER

# 2. Setup / Configure Meson
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}[*] Creating build directory and configuring...${NC}"
    # Initial setup
    meson setup "$BUILD_DIR" --buildtype=release
else
    echo -e "${YELLOW}[*] Re-configuring existing build directory...${NC}"
    # Reconfigure ensures changes to meson.build or compiler are picked up
    meson setup --reconfigure "$BUILD_DIR" --buildtype=release
fi

# 3. Compile
echo -e "${YELLOW}[*] Compiling keysniff...${NC}"
meson compile -C "$BUILD_DIR"

# 4. Success Message
echo -e "${GREEN}[+] Build successful!${NC}"
echo -e "    Binary location: ./${BUILD_DIR}/${BINARY_NAME}"

# 5. Display binary info to verify it's 64-bit and stripped
echo -e "${YELLOW}[*] Binary info:${NC}"
file "${BUILD_DIR}/${BINARY_NAME}"