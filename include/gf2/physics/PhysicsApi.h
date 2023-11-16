// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_API_H
#define GF_PHYSICS_API_H

#include <gf2/core/Portability.h>

#ifndef GF_PHYSICS_STATIC
#  ifdef GF_PHYSICS_BUILD
#    define GF_PHYSICS_API GF_EXPORT
#  else
#    define GF_PHYSICS_API GF_IMPORT
#  endif
#else
#  define GF_PHYSICS_API
#endif

#endif // GF_PHYSICS_API_H
