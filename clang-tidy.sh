#!/bin/sh

clang-tidy -header-filter=.* -p build include/gf2/*.h library/core/*.cc
