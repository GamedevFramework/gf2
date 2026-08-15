// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGET_DATA_H
#define GF_WIDGET_DATA_H

#include "CoreApi.h"
#include "TextData.h"
#include "TypeTraits.h"

namespace gf {

  /*
   * TextWidget
   */

  struct GF_CORE_API TextWidgetData {
    TextData disabled_text;
    TextData default_text;
    TextData selected_text;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextWidgetData, Archive>& data)
  {
    return ar | data.disabled_text | data.default_text | data.selected_text;
  }

  struct GF_CORE_API TextWidgetResource {
    std::filesystem::path font;
    TextWidgetData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextWidgetResource, Archive>& resource)
  {
    return ar | resource.font | resource.data;
  }

}

#endif // GF_WIDGET_DATA_H
