// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PORTABILITY_H
#define GF_PORTABILITY_H

#ifdef __GNUC__
  #define GF_FORMAT(X, Y) __attribute__ ((format (printf, X, Y)))
#else
  #define GF_FORMAT(X, Y)
#endif

#endif // GF_PORTABILITY_H
