// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TILED_MAP_RESOURCE_H
#define GF_TILED_MAP_RESOURCE_H

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API TiledMapResource {
    std::filesystem::path filename;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<TiledMapResource, Archive>& resource)
  {
    return ar | resource.filename;
  }

}

#endif // GF_TILED_MAP_RESOURCE_H
