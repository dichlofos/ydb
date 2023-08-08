# syntax=docker/dockerfile:1.0
FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

## prepare system
RUN apt-get update \
    && apt-get install -y wget gnupg lsb-release curl xz-utils tzdata \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - \
    && echo "deb http://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-14 main" | tee /etc/apt/sources.list.d/llvm.list >/dev/null \
    && apt-get update


RUN apt-get install -y git cmake python3-pip ninja-build antlr3 m4 clang-14 lld-14 libidn11-dev libaio1 libaio-dev llvm-14 make \
    && pip3 install conan==1.59 \
    && (V=4.8.1; curl -L https://github.com/ccache/ccache/releases/download/v${V}/ccache-${V}-linux-x86_64.tar.xz | \
     tar -xJ -C /usr/local/bin/ --strip-components=1 --no-same-owner ccache-${V}-linux-x86_64/ccache)
