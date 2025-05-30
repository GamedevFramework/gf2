// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_DATA_H
#define GF_ANIMATION_DATA_H

#include <cstdint>

#include <filesystem>
#include <map>
#include <vector>

#include "Color.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Id.h"
#include "Rect.h"
#include "Time.h"
#include "TypeTraits.h"

namespace gf {

  enum class AnimationFrameFlip : uint8_t {
    Horizontally = 0x01,
    Vertically = 0x02,
    Diagonally = 0x04,
  };

  template<>
  struct EnableBitmaskOperators<AnimationFrameFlip> : std::true_type {
  };

  struct GF_CORE_API AnimationFrameData {
    uint32_t texture_index = 0;
    RectF texture_region = RectF::from_size({ 0.0f, 0.0f });
    Time duration;
    Flags<AnimationFrameFlip> flip = None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<AnimationFrameData, Archive>& data)
  {
    return ar | data.texture_index | data.texture_region | data.duration | data.flip;
  }

  enum class AnimationProperties : uint8_t {
    Loop = 0x01,
  };

  template<>
  struct EnableBitmaskOperators<AnimationProperties> : std::true_type {
  };

  struct GF_CORE_API AnimationData {
    std::vector<AnimationFrameData> frames;
    Flags<AnimationProperties> properties = None;
    Color color = White;

    void add_tileset(uint32_t texture_index, Vec2I layout, Time duration, int32_t frame_count, int32_t frame_offset = 0, int32_t frame_direction = 1);
    void add_tileset(uint32_t texture_index, Vec2I layout, Time duration, Flags<AnimationFrameFlip> flip, int32_t frame_count, int32_t frame_offset = 0, int32_t frame_direction = 1);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<AnimationData, Archive>& data)
  {
    return ar | data.frames | data.properties | data.color;
  }

  struct GF_CORE_API AnimationResource {
    std::vector<std::filesystem::path> textures;
    AnimationData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<AnimationResource, Archive>& resource)
  {
    return ar | resource.textures | resource.data;
  }

  struct GF_CORE_API AnimationGroupData {
    std::map<Id, AnimationData> animations;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<AnimationGroupData, Archive>& data)
  {
    return ar | data.animations;
  }

  struct GF_CORE_API AnimationGroupResource {
    std::vector<std::filesystem::path> textures;
    AnimationGroupData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<AnimationGroupResource, Archive>& resource)
  {
    return ar | resource.textures | resource.data;
  }

}

#endif // GF_ANIMATION_DATA_H
