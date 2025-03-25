// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_FONT_RESOURCE_H
#define GF_FONT_RESOURCE_H

#include <filesystem>

#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API FontResource {
    std::filesystem::path path;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<FontResource, Archive>& resource)
  {
    return ar | resource.path;
  }

}

#endif // GF_FONT_RESOURCE_H
