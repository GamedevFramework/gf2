// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_AUDIO_SOURCE_DATA_H
#define GF_AUDIO_SOURCE_DATA_H

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API AudioSourceData {
    float volume = 1.0f;
    bool loop = false;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<AudioSourceData, Archive>& data)
  {
    return ar | data.volume | data.loop;
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
