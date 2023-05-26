#ifndef GF_AUDIO_BACKEND_H
#define GF_AUDIO_BACKEND_H

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#ifdef GF_AUDIO_BACKEND_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#endif

// #define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
// #define MA_ENABLE_CUSTOM
#define MA_DEBUG_OUTPUT

#include <miniaudio.h>

#ifdef GF_AUDIO_BACKEND_IMPLEMENTATION
#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>
#endif

#endif // GF_AUDIO_BACKEND_H
