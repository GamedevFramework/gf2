// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
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

#ifdef _MSC_VER
#  define GF_FIX_DUMB_MSVC(name)                  \
     name(const name&) = delete;                  \
     name(name&&) noexcept = default;             \
     ~name() = default;                           \
     name& operator=(const name&) = delete;       \
     name& operator=(name&&) noexcept = default;

#  define GF_FIX_DUMB_MSVC_FULL(name)             \
     name() = default;                            \
     GF_FIX_DUMB_MSVC(name)
#else
#  define GF_FIX_DUMB_MSVC(name)
#  define GF_FIX_DUMB_MSVC_FULL(name)
#endif


#endif // GF_PORTABILITY_H
