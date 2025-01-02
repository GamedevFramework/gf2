// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_FRAMEWORK_API_H
#define GF_FRAMEWORK_API_H

#include <gf2/core/Portability.h>

#ifndef GF_FRAMEWORK_STATIC
#  ifdef GF_FRAMEWORK_BUILD
#    define GF_FRAMEWORK_API GF_EXPORT
#  else
#    define GF_FRAMEWORK_API GF_IMPORT
#  endif
#else
#  define GF_FRAMEWORK_API
#endif

#endif // GF_FRAMEWORK_API_H
