#!/bin/sh

clang-tidy -header-filter=.* -p build include/gf2/**/*.h library/core/*.cc library/graphics/*.cc library/network/*.cc library/network/bits/* examples/*.cc bin/*.cc
