// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGET_DATA_H
#define GF_WIDGET_DATA_H

#include "CoreApi.h"
#include "SpriteData.h"
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

  /*
   * TextButtonWidget
   */

  struct GF_CORE_API ButtonWidgetData {
    Color color = Black;
    float outline_thickness = 0.0f;
    Color outline_color = White;
    float border_padding = 0.0f;
    float corner_radius = 0.0f;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ButtonWidgetData, Archive>& data)
  {
    return ar | data.color | data.outline_thickness | data.outline_color | data.border_padding | data.corner_radius;
  }

  struct GF_CORE_API TextButtonWidgetData : TextWidgetData {
    ButtonWidgetData disabled_button;
    ButtonWidgetData default_button;
    ButtonWidgetData selected_button;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextButtonWidgetData, Archive>& data)
  {
    MaybeConst<TextWidgetData, Archive>& text_widget_data = data;
    return ar | text_widget_data | data.disabled_button | data.default_button | data.selected_button;
  }

  struct GF_CORE_API TextButtonWidgetResource {
    std::filesystem::path font;
    TextButtonWidgetData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<TextButtonWidgetResource, Archive>& resource)
  {
    return ar | resource.font | resource.data;
  }

  /*
   * SpriteWidget
   */

  struct GF_CORE_API SpriteWidgetData {
    SpriteData disabled_sprite;
    SpriteData default_sprite;
    SpriteData selected_sprite;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteWidgetData, Archive>& data)
  {
    return ar | data.disabled_sprite | data.default_sprite | data.selected_sprite;
  }

  struct GF_CORE_API SpriteWidgetResource {
    std::filesystem::path disabled_texture;
    std::filesystem::path default_texture;
    std::filesystem::path selected_texture;
    SpriteWidgetData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<SpriteWidgetResource, Archive>& resource)
  {
    return ar | resource.disabled_texture | resource.default_texture | resource.selected_texture | resource.data;
  }

}

#endif // GF_WIDGET_DATA_H
