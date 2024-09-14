// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_NINE_PATCH_DATA_H
#define GF_NINE_PATCH_DATA_H

#include <filesystem>

#include "Color.h"
#include "CoreApi.h"
#include "Rect.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API NinePatchLimits {
    float top = 0.25f;
    float bottom = 0.25f;
    float left = 0.25f;
    float right = 0.25f;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<NinePatchLimits, Archive>& limits)
  {
    return ar | limits.top | limits.bottom | limits.left | limits.right;
  }

  struct GF_CORE_API NinePatchData {
    Vec2F size = { 1.0f, 1.0f };
    NinePatchLimits limits;
    RectF texture_region = RectF::from_size({ 1.0f, 1.0f });
    Color color = White;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<NinePatchData, Archive>& data)
  {
    return ar | data.size | data.limits | data.texture_region | data.color;
  }

  struct GF_CORE_API NinePatchResource {
    std::filesystem::path texture;
    NinePatchData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<NinePatchResource, Archive>& resource)
  {
    return ar | resource.texture | resource.data;
  }

}

#endif // GF_NINE_PATCH_DATA_H
