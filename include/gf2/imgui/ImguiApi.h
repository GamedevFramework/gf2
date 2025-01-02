// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_IMGUI_API_H
#define GF_IMGUI_API_H

#include <gf2/core/Portability.h>

#ifndef GF_IMGUI_STATIC
#  ifdef GF_IMGUI_BUILD
#    define GF_IMGUI_API GF_EXPORT
#  else
#    define GF_IMGUI_API GF_IMPORT
#  endif
#else
#  define GF_IMGUI_API
#endif

#endif // GF_IMGUI_API_H
