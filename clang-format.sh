#!/bin/sh

clang-format -i include/gf2/*.h library/core/*.cc library/graphics/*.cc library/network/*.cc library/network/bits/* tests/*.cc
