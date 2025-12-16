#!/bin/bash
set -e

IMAGE_NAME="keysniff-builder"
OUTPUT_DIR="bin"
CROSS_FILE="cross_comp.txt"

# Docker Internal

if [ "$1" == "--internal" ]; then

    mkdir -p "$OUTPUT_DIR"

    export CC=clang
    export CXX=clang++
    
    if [ -d "build_linux" ]; then rm -rf build_linux; fi

    meson setup "build_linux" --buildtype=release
    meson compile -C "build_linux"
    
    cp "build_linux/keysniff" "$OUTPUT_DIR/keysniff_linux"

    cat <<EOF > "$CROSS_FILE"
[binaries]
c = 'x86_64-w64-mingw32-gcc'
cpp = 'x86_64-w64-mingw32-g++'
ar = 'x86_64-w64-mingw32-ar'
strip = 'x86_64-w64-mingw32-strip'
pkg-config = 'x86_64-w64-mingw32-pkg-config'

[host_machine]
system = 'windows'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
EOF

    if [ ! -f "subprojects/curl.wrap" ]; then
        mkdir -p subprojects
        meson wrap install curl
    fi

    if [ -d "build_windows" ]; then rm -rf build_windows; fi
    
    meson setup "build_windows" \
        --cross-file "$CROSS_FILE" \
        --buildtype=release \
        --force-fallback-for=libcurl \
        \
        -Dcurl:tool=disabled \
        -Dcurl:tests=disabled \
        -Dcurl:unittests=disabled \
        \
        -Dcurl:ssl=enabled \
        -Dcurl:openssl=disabled \
        -Dcurl:schannel=enabled \
        -Dcurl:ssl-default-backend=schannel \
        \
        -Dcurl:http=enabled \
        -Dcurl:ftp=disabled \
        -Dcurl:file=disabled \
        -Dcurl:dict=disabled \
        -Dcurl:gopher=disabled \
        -Dcurl:imap=disabled \
        -Dcurl:ldap=disabled \
        -Dcurl:ldaps=disabled \
        -Dcurl:mqtt=disabled \
        -Dcurl:pop3=disabled \
        -Dcurl:rtmp=disabled \
        -Dcurl:rtsp=disabled \
        -Dcurl:smb=disabled \
        -Dcurl:smtp=disabled \
        -Dcurl:telnet=disabled \
        -Dcurl:tftp=disabled \
        \
        -Dcurl:http2=disabled \
        -Dcurl:brotli=disabled \
        -Dcurl:zstd=disabled \
        -Dcurl:libz=disabled \
        -Dcurl:psl=disabled \
        -Dcurl:ssh=disabled \
        -Dcurl:idn=disabled \
        \
        -Dcurl:cookies=disabled \
        -Dcurl:proxy=enabled \
        -Dcurl:ipv6=enabled \
        -Dcurl:mime=disabled \
        -Dcurl:form-api=disabled \
        -Dcurl:netrc=disabled \
        -Dcurl:progress-meter=disabled \
        -Dcurl:verbose-strings=disabled \
        \
        -Dcurl:gss-api=disabled \
        -Dcurl:ntlm=disabled \
        -Dcurl:kerberos-auth=disabled \
        -Dcurl:negotiate-auth=disabled \
        -Dcurl:aws=disabled \
        -Dcurl:basic-auth=disabled \
        -Dcurl:bearer-auth=disabled \
        -Dcurl:digest-auth=disabled

    meson compile -C "build_windows"

    cp "build_windows/keysniff.exe" "$OUTPUT_DIR/keysniff.exe"

    exit 0
fi

# External

find src include -name "*.[ch]" | xargs clang-format -i -style=file

if ! command -v docker &> /dev/null; then
    echo -e "You need to have Docker installed on your system."
    exit 1
fi

docker build -t "$IMAGE_NAME" .

docker run --rm \
    -v "$(pwd):/app" \
    "$IMAGE_NAME" \
    ./build.sh --internal

if [ -d "$OUTPUT_DIR" ]; then
    USER_ID=$(id -u)
    GROUP_ID=$(id -g)
    sudo chown -R "$USER_ID:$GROUP_ID" "$OUTPUT_DIR" "build_linux" "build_windows" "subprojects" "src" "include" 2>/dev/null || true
fi

ls -l "$OUTPUT_DIR"

file "$OUTPUT_DIR/keysniff_linux"
file "$OUTPUT_DIR/keysniff.exe"
