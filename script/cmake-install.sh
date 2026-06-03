#! /bin/sh

mkdir -p build
cmake -B build
cmake --build build

# TODO: Stop hard-coding sudo
sudo cmake --install build
