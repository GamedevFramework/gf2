#!/bin/sh

clang-format -i include/gf2/**/*.h library/audio/*.cc library/core/*.cc library/graphics/*.cc library/network/*.cc library/network/bits/* library/imgui/*.cc tests/*.cc examples/*.cc bin/*.cc games/**/*.cc games/**/**/*.h games/**/**/*.cc
