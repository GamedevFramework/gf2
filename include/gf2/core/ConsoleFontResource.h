// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_FONT_RESOURCE_H
#define GF_CONSOLE_FONT_RESOURCE_H

#include <filesystem>

#include "ConsoleFontData.h"
#include "CoreApi.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API ConsoleFontResource {
    std::filesystem::path console_font;
    ConsoleFontData data;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ConsoleFontResource, Archive>& resource)
  {
    return ar | resource.console_font | resource.data;
  }

  struct GF_CORE_API MixedConsoleFontResource {
    ConsoleFontResource picture;
    ConsoleFontResource text;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<MixedConsoleFontResource, Archive>& resource)
  {
    return ar | resource.picture | resource.text;
  }

}

#endif // GF_CONSOLE_FONT_RESOURCE_H
