#!/bin/sh

clang-tidy -p build include/gf2/**/*.h library/core/*.cc library/graphics/*.cc library/network/*.cc library/network/bits/* library/physics/*.cc library/imgui/*.cc examples/*.cc bin/*.cc  games/**/*.cc games/**/**/*.h games/**/**/*.cc
