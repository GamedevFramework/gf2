// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Color.h>
// clang-format on

#include <cassert>
#include <cmath>

#include <algorithm>
#include <limits>

namespace gf {

  namespace {

    struct HSV {
      float h;
      float s;
      float v;
      float a;
    };

    HSV rgb_to_hsv(Color color)
    {
      const float min = std::min({ color.r, color.g, color.b });
      const float max = std::max({ color.r, color.g, color.b });

      float hue = 0;

      if ((max - min) > std::numeric_limits<float>::epsilon()) {
        if (max == color.r) {
          hue = std::fmod(60.0f * (color.g - color.b) / (max - min) + 360.0f, 360.0f);
        } else if (max == color.g) {
          hue = 60.0f * (color.b - color.r) / (max - min) + 120.0f;
        } else if (max == color.b) {
          hue = 60.0f * (color.r - color.g) / (max - min) + 240.0f;
        } else {
          assert(false);
        }
      }

      const float sat = (max < std::numeric_limits<float>::epsilon()) ? 0.0f : (1.0f - min / max);
      const float val = max;

      return { hue, sat, val, color.a };
    }

    Color hsv_to_rgb(HSV hsv)
    {
      const float hue = hsv.h / 60.0f;
      const float sat = hsv.s;
      const float val = hsv.v;

      const int i = static_cast<int>(hue) % 6;
      assert(0 <= i && i < 6);

      const float f = hue - static_cast<float>(i);

      const float x = val * (1.0f - sat);
      const float y = val * (1.0f - (f * sat));
      const float z = val * (1.0f - ((1.0f - f) * sat));

      Color color(0.0f, 0.0f, 0.0f);
      color.a = hsv.a;

      switch (i) {
        case 0:
          color.r = val;
          color.g = z;
          color.b = x;
          break;
        case 1:
          color.r = y;
          color.g = val;
          color.b = x;
          break;
        case 2:
          color.r = x;
          color.g = val;
          color.b = z;
          break;
        case 3:
          color.r = x;
          color.g = y;
          color.b = val;
          break;
        case 4:
          color.r = z;
          color.g = x;
          color.b = val;
          break;
        case 5:
          color.r = val;
          color.g = x;
          color.b = y;
          break;
        default:
          assert(false);
          break;
      }

      return color;
    }

  } // namespace

  Color lighter(Color color, float percent)
  {
    assert(0.0f <= percent && percent <= 1.0f);
    auto hsv = rgb_to_hsv(color);
    hsv.v += hsv.v * percent;

    if (hsv.v > 1) {
      hsv.s -= (hsv.v - 1);

      if (hsv.s < 0) {
        hsv.s = 0;
      }

      hsv.v = 1;
    }

    return hsv_to_rgb(hsv);
  }

  Color darker(Color color, float percent)
  {
    assert(0.0f <= percent && percent <= 1.0f);
    auto hsv = rgb_to_hsv(color);
    hsv.v -= hsv.v * percent;
    return hsv_to_rgb(hsv);
  }

  namespace {
    float srgb_to_linear(float c)
    {
      return (c < 0.04045f) ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
    }

    float linear_to_srgb(float c)
    {
      return (c < 0.0031308f) ? 12.92f * c : 1.055f * std::pow(c, 1.f / 2.4f) - 0.055f;
    }
  }

  Color srgb_to_linear(Color color)
  {
    return { srgb_to_linear(color.r), srgb_to_linear(color.g), srgb_to_linear(color.b), color.a };
  }

  Color linear_to_srgb(Color color)
  {
    return { linear_to_srgb(color.r), linear_to_srgb(color.g), linear_to_srgb(color.b), color.a };
  }

} // namespace gf
