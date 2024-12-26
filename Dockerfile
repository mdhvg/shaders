FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    mingw-w64 \
    cmake \
    ninja-build \
    && apt-get clean

WORKDIR /build
