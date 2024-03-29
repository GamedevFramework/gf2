// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_COLOR_H
#define GF_COLOR_H

#include <cstdint>

#include "CoreApi.h"
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
    : r(static_cast<float>((rgb >> 16) & 0xFF) / 255.0f)
    , g(static_cast<float>((rgb >> 8)  & 0xFF) / 255.0f)
    , b(static_cast<float>((rgb >> 0)  & 0xFF) / 255.0f)
    // clang-format on
    , a(static_cast<float>(a) / 255.0f)
    {
    }

    float r;
    float g;
    float b;
    float a;
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
  inline constexpr Color Green(0.0f, 1.0f, 0.0f);
  inline constexpr Color Blue(0.0f, 0.0f, 1.0f);
  inline constexpr Color Cyan(0.0f, 1.0f, 1.0f);
  inline constexpr Color Magenta(1.0f, 0.0f, 1.0f);
  inline constexpr Color Yellow(1.0f, 1.0f, 0.0f);

  inline constexpr Color Orange(1.0f, 0.5f, 0.0f);
  inline constexpr Color Rose(1.0f, 0.0f, 0.5f);
  inline constexpr Color Chartreuse(0.5f, 1.0f, 0.0f);
  inline constexpr Color Spring(0.0f, 1.0f, 0.5f);
  inline constexpr Color Violet(0.5f, 0.0f, 1.0f);
  inline constexpr Color Azure(0.0f, 0.5f, 1.0f);

  inline constexpr Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<Color, Archive>& color)
  {
    return ar | color.r | color.g | color.b | color.a;
  }

} // namespace gf

#endif // GF_COLOR_H
