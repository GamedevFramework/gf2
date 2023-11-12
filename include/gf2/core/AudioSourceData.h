// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_SOURCE_DATA_H
#define GF_AUDIO_SOURCE_DATA_H

#include <cstdint>

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  enum class AudioSourceType : uint8_t {
    Sound,
    Music,
  };

  struct GF_CORE_API AudioSourceData {
    AudioSourceType type = AudioSourceType::Sound;
    bool loop = false;
    float volume = 1.0f;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<AudioSourceData, Archive>& data)
  {
    return ar | data.type | data.volume | data.loop;
  }

  struct GF_CORE_API AudioSourceResource {
    std::filesystem::path filename;
    AudioSourceData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<AudioSourceResource, Archive>& resource)
  {
    return ar | resource.filename | resource.data;
  }

}

#endif // GF_AUDIO_SOURCE_DATA_H
