// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PORTABILITY_H
#define GF_PORTABILITY_H

#if defined _WIN32 || defined __CYGWIN__
#  ifdef __GNUC__
#    define GF_EXPORT __attribute__((dllexport))
#    define GF_IMPORT __attribute__((dllimport))
#  else
#    define GF_EXPORT __declspec(dllexport)
#    define GF_IMPORT __declspec(dllimport)
#  endif
#else
#  if __GNUC__ >= 4
#    define GF_EXPORT __attribute__((visibility("default")))
#    define GF_IMPORT __attribute__((visibility("default")))
#  else
#    define GF_EXPORT
#    define GF_IMPORT
#  endif
#endif

#endif // GF_PORTABILITY_H
