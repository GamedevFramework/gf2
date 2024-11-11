// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SPRITE_DATA_H
#define GF_SPRITE_DATA_H

#include <filesystem>
#include <vector>

#include "Color.h"
#include "CoreApi.h"
#include "Rect.h"
#include "Transform.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API SpriteData {
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
    Color color = White;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteData, Archive>& data)
  {
    return ar | data.texture_region | data.color;
  }

  struct GF_CORE_API SpriteResource {
    std::filesystem::path texture;
    SpriteData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteResource, Archive>& resource)
  {
    return ar | resource.texture | resource.data;
  }

  struct GF_CORE_API IndexedSpriteData {
    uint32_t texture_index = 0;
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
    Color color = White;
    Transform transform;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<IndexedSpriteData, Archive>& data)
  {
    return ar | data.texture_index | data.texture_region | data.color | data.transform;
  }

  struct GF_CORE_API SpriteGroupData {
    std::vector<IndexedSpriteData> sprites;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteGroupData, Archive>& data)
  {
    return ar | data.sprites;
  }

  struct GF_CORE_API SpriteGroupResource {
    uint32_t add_texture(const std::filesystem::path& texture_path);
    void add_sprite(const SpriteResource& resource, const Transform& transform);

    std::vector<std::filesystem::path> textures;
    SpriteGroupData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteGroupResource, Archive>& resource)
  {
    return ar | resource.textures | resource.data;
  }

}

#endif // GF_SPRITE_DATA_H
