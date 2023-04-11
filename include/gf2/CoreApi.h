// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CORE_API_H
#define GF_CORE_API_H

#include "Portability.h"

#ifndef GF_CORE_STATIC
  #ifdef GF_CORE_BUILD
    #define GF_CORE_API GF_EXPORT
  #else
    #define GF_CORE_API GF_IMPORT
  #endif
#else
  #define GF_CORE_API
#endif

#endif // GF_CORE_API_H
