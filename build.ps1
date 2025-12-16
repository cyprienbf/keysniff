# ========================================
#  Compile Script for Windows (PowerShell)
# ========================================

# Configuration
$BuildDir = "bin"
$BinaryName = "keysniff.exe" 
$Compiler = "clang"

$ErrorActionPreference = "Stop"

$env:CC = $Compiler

# Find .c and .h files recursively in src/ and include/ and format them
$SourceDirs = @("src", "include") | Where-Object { Test-Path $_ }

if ($SourceDirs) {
    Get-ChildItem -Path $SourceDirs -Recurse -Include *.c, *.h | ForEach-Object {
        clang-format -i -style=file $_.FullName
    }
}

# Setup / Configure Meson
if (-not (Test-Path -Path $BuildDir)) {
    meson setup $BuildDir --buildtype=release
} else {
    meson setup --reconfigure $BuildDir --buildtype=release
}

# Compile
meson compile -C $BuildDir
