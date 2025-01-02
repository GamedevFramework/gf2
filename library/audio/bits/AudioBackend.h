// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_AUDIO_BACKEND_H
#define GF_AUDIO_BACKEND_H

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#ifdef GF_AUDIO_BACKEND_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#endif

#include <miniaudio.h>

#ifdef GF_AUDIO_BACKEND_IMPLEMENTATION
#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>
#endif

#endif // GF_AUDIO_BACKEND_H
