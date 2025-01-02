// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GRAPHICS_API_H
#define GF_GRAPHICS_API_H

#include <gf2/core/Portability.h>

#ifndef GF_GRAPHICS_STATIC
#  ifdef GF_GRAPHICS_BUILD
#    define GF_GRAPHICS_API GF_EXPORT
#  else
#    define GF_GRAPHICS_API GF_IMPORT
#  endif
#else
#  define GF_GRAPHICS_API
#endif

#endif // GF_GRAPHICS_API_H
