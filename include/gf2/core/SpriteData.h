// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SPRITE_DATA_H
#define GF_SPRITE_DATA_H

#include <filesystem>

#include "Color.h"
#include "CoreApi.h"
#include "Rect.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API SpriteData {
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
    Color color = White;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<SpriteData, Archive>& data)
  {
    return ar | data.texture_region | data.color;
  }

  struct GF_CORE_API SpriteResource {
    std::filesystem::path texture;
    SpriteData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<SpriteResource, Archive>& resource)
  {
    return ar | resource.texture | resource.data;
  }

}

#endif // GF_SPRITE_DATA_H
