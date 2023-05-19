// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_NETWORK_API_H
#define GF_NETWORK_API_H

#include "Portability.h"

#ifndef GF_NETWORK_STATIC
#  ifdef GF_NETWORK_BUILD
#    define GF_NETWORK_API GF_EXPORT
#  else
#    define GF_NETWORK_API GF_IMPORT
#  endif
#else
#  define GF_NETWORK_API
#endif

#endif // GF_NETWORK_API_H
