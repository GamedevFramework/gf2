// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_AUDIO_API_H
#define GF_AUDIO_API_H

#include <gf2/core/Portability.h>

#ifndef GF_AUDIO_STATIC
#  ifdef GF_AUDIO_BUILD
#    define GF_AUDIO_API GF_EXPORT
#  else
#    define GF_AUDIO_API GF_IMPORT
#  endif
#else
#  define GF_AUDIO_API
#endif

#endif // GF_AUDIO_API_H
