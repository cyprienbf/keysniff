FROM debian:stable-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    meson \
    ninja-build \
    pkg-config \
    clang \
    clang-format \
    lld \
    llvm \
    mingw-w64 \
    libcurl4-openssl-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

CMD ["/bin/bash"]