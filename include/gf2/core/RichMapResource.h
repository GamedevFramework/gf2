// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RICH_MAP_RESOURCE_H
#define GF_RICH_MAP_RESOURCE_H

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API RichMapResource {
    std::filesystem::path filename;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<RichMapResource, Archive>& resource)
  {
    return ar | resource.filename;
  }

}

#endif // GF_RICH_MAP_RESOURCE_H
