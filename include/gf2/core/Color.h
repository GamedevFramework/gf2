// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_COLOR_H
#define GF_COLOR_H

#include <cstdint>

#include <array>

#include "CoreApi.h"
#include "Math.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API Color {
    constexpr Color()
    : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
    {
    }

    constexpr Color(float r, float g, float b, float a = 1.0f)
    : r(r), g(g), b(b), a(a)
    {
    }

    constexpr Color(uint32_t rgb, uint8_t a = 255)
    // clang-format off
    : r(float((rgb >> 16) & 0xFF) / 255.0f)
    , g(float((rgb >> 8)  & 0xFF) / 255.0f)
    , b(float((rgb >> 0)  & 0xFF) / 255.0f)
    // clang-format on
    , a(float(a) / 255.0f)
    {
    }

    float r;
    float g;
    float b;
    float a;

    constexpr std::array<uint8_t, 4> to_rgba32() const
    {
      return { uint8_t(r * 255), uint8_t(g * 255), uint8_t(b * 255), uint8_t(a * 255) };
    }

    constexpr uint32_t to_hex() const
    {
      const auto rgba = to_rgba32();
      return (uint32_t(rgba[0]) << 24) + (uint32_t(rgba[1]) << 16) + (uint32_t(rgba[2]) << 8) + (uint32_t(rgba[3]) << 0);
    }
  };

  constexpr bool operator==(Color lhs, Color rhs)
  {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
  }

  constexpr bool operator!=(Color lhs, Color rhs)
  {
    return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.a != rhs.a;
  }

  /*
   * Addition
   */

  constexpr Color operator+(Color lhs, Color rhs)
  {
    return { lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a };
  }

  constexpr Color& operator+=(Color& lhs, Color rhs)
  {
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    lhs.a += rhs.a;
    return lhs;
  }

  constexpr Color operator+(Color lhs, float rhs)
  {
    return { lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a + rhs };
  }

  constexpr Color& operator+=(Color& lhs, float rhs)
  {
    lhs.r += rhs;
    lhs.g += rhs;
    lhs.b += rhs;
    lhs.a += rhs;
    return lhs;
  }

  constexpr Color operator+(float lhs, Color rhs)
  {
    return { lhs + rhs.r, lhs + rhs.g, lhs + rhs.b, lhs + rhs.a };
  }

  /*
   * Substraction
   */

  constexpr Color operator-(Color lhs, Color rhs)
  {
    return { lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a };
  }

  constexpr Color& operator-=(Color& lhs, Color rhs)
  {
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;
    lhs.a -= rhs.a;
    return lhs;
  }

  constexpr Color operator-(Color lhs, float rhs)
  {
    return { lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a - rhs };
  }

  constexpr Color& operator-=(Color& lhs, float rhs)
  {
    lhs.r -= rhs;
    lhs.g -= rhs;
    lhs.b -= rhs;
    lhs.a -= rhs;
    return lhs;
  }

  constexpr Color operator-(float lhs, Color rhs)
  {
    return { lhs - rhs.r, lhs - rhs.g, lhs - rhs.b, lhs - rhs.a };
  }

  /*
   * Multiplication
   */

  constexpr Color operator*(Color lhs, Color rhs)
  {
    return { lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a };
  }

  constexpr Color& operator*=(Color& lhs, Color rhs)
  {
    lhs.r *= rhs.r;
    lhs.g *= rhs.g;
    lhs.b *= rhs.b;
    lhs.a *= rhs.a;
    return lhs;
  }

  constexpr Color operator*(Color lhs, float rhs)
  {
    return { lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs };
  }

  constexpr Color& operator*=(Color& lhs, float rhs)
  {
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    lhs.a *= rhs;
    return lhs;
  }

  constexpr Color operator*(float lhs, Color rhs)
  {
    return { lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a };
  }

  /*
   * Division
   */

  constexpr Color operator/(Color lhs, Color rhs)
  {
    return { lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a };
  }

  constexpr Color& operator/=(Color& lhs, Color rhs)
  {
    lhs.r /= rhs.r;
    lhs.g /= rhs.g;
    lhs.b /= rhs.b;
    lhs.a /= rhs.a;
    return lhs;
  }

  constexpr Color operator/(Color lhs, float rhs)
  {
    return { lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs };
  }

  constexpr Color& operator/=(Color& lhs, float rhs)
  {
    lhs.r /= rhs;
    lhs.g /= rhs;
    lhs.b /= rhs;
    lhs.a /= rhs;
    return lhs;
  }

  constexpr Color operator/(float lhs, Color rhs)
  {
    return { lhs / rhs.r, lhs / rhs.g, lhs / rhs.b, lhs / rhs.a };
  }

  GF_CORE_API Color lighter(Color color, float percent = 0.5f);
  GF_CORE_API Color darker(Color color, float percent = 0.5f);

  GF_CORE_API Color srgb_to_linear(Color color);
  GF_CORE_API Color linear_to_srgb(Color color);

  constexpr Color max(Color lhs, Color rhs)
  {
    return { details::max(lhs.r, rhs.r), details::max(lhs.g, rhs.g), details::max(lhs.b, rhs.b), details::max(lhs.a, rhs.a) };
  }

  constexpr Color min(Color lhs, Color rhs)
  {
    return { details::min(lhs.r, rhs.r), details::min(lhs.g, rhs.g), details::min(lhs.b, rhs.b), details::min(lhs.a, rhs.a) };
  }

  constexpr Color clamp(Color color, float min = 0.0f, float max = 1.0f)
  {
    return { details::clamp(color.r, min, max), details::clamp(color.g, min, max), details::clamp(color.b, min, max), details::clamp(color.a, min, max) };
  }

  constexpr Color opaque(float value = 0.5f)
  {
    return { 1.0f, 1.0f, 1.0f, value };
  }

  constexpr Color gray(float value = 0.5f)
  {
    return { value, value, value, 1.0f };
  }

  inline constexpr Color Black(0.0f, 0.0f, 0.0f);
  inline constexpr Color White(1.0f, 1.0f, 1.0f);
  inline constexpr Color Gray = gray();

  inline constexpr Color Red(1.0f, 0.0f, 0.0f);
  inline constexpr Color Vermilion(1.0f, 0.25f, 0.0f);
  inline constexpr Color Orange(1.0f, 0.5f, 0.0f);
  inline constexpr Color Amber(1.0f, 0.75f, 0.0f);
  inline constexpr Color Yellow(1.0f, 1.0f, 0.0f);
  inline constexpr Color Lime(0.75f, 1.0f, 0.0f);
  inline constexpr Color Chartreuse(0.5f, 1.0f, 0.0f);
  inline constexpr Color Harlequin(0.25f, 1.0f, 0.0f);
  inline constexpr Color Green(0.0f, 1.0f, 0.0f);
  inline constexpr Color Erin(0.0f, 1.0f, 0.25f);
  inline constexpr Color Spring(0.0f, 1.0f, 0.5f);
  inline constexpr Color Aquamarine(0.0f, 1.0f, 0.75f);
  inline constexpr Color Cyan(0.0f, 1.0f, 1.0f);
  inline constexpr Color Capri(0.0f, 0.75f, 1.0f);
  inline constexpr Color Azure(0.0f, 0.5f, 1.0f);
  inline constexpr Color Cerulean(0.0f, 0.25f, 1.0f);
  inline constexpr Color Blue(0.0f, 0.0f, 1.0f);
  inline constexpr Color Indigo(0.25f, 0.0f, 1.0f);
  inline constexpr Color Violet(0.5f, 0.0f, 1.0f);
  inline constexpr Color Purple(0.75f, 0.0f, 1.0f);
  inline constexpr Color Magenta(1.0f, 0.0f, 1.0f);
  inline constexpr Color Cerise(1.0f, 0.0f, 0.75f);
  inline constexpr Color Rose(1.0f, 0.0f, 0.5f);
  inline constexpr Color Crimson(1.0f, 0.0f, 0.25f);

  inline constexpr Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<Color, Archive>& color)
  {
    return ar | color.r | color.g | color.b | color.a;
  }

} // namespace gf

#endif // GF_COLOR_H
