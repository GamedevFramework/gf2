// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_COLOR_H
#define GF_COLOR_H

#include <cstdint>

#include "CoreApi.h"

namespace gf {

  struct GF_CORE_API Color {
    constexpr
    Color()
    : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
    {
    }

    constexpr
    Color(float r, float g, float b, float a = 1.0f)
    : r(r), g(g), b(b), a(a)
    {
    }

    constexpr
    Color(uint32_t rgb, uint8_t a = 255)
    : r(((rgb >> 16) & 0xFF) / 255.0f)
    , g(((rgb >> 8)  & 0xFF) / 255.0f)
    , b(((rgb >> 0)  & 0xFF) / 255.0f)
    , a(a / 255.0f)
    {
    }

    float r;
    float g;
    float b;
    float a;
  };

  constexpr
  bool operator==(Color lhs, Color rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
  }

  constexpr
  bool operator!=(Color lhs, Color rhs) {
    return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.a != rhs.a;
  }

  /*
   * Addition
   */

  constexpr
  Color operator+(Color lhs, Color rhs) {
    return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
  }

  constexpr
  Color& operator+=(Color& lhs, Color rhs) {
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    lhs.a += rhs.a;
    return lhs;
  }

  constexpr
  Color operator+(Color lhs, float rhs) {
    return Color(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a + rhs);
  }

  constexpr
  Color& operator+=(Color& lhs, float rhs) {
    lhs.r += rhs;
    lhs.g += rhs;
    lhs.b += rhs;
    lhs.a += rhs;
    return lhs;
  }

  constexpr
  Color operator+(float lhs, Color rhs) {
    return Color(lhs + rhs.r, lhs + rhs.g, lhs + rhs.b, lhs + rhs.a);
  }

  /*
   * Substraction
   */

  constexpr
  Color operator-(Color lhs, Color rhs) {
    return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
  }

  constexpr
  Color& operator-=(Color& lhs, Color rhs) {
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;
    lhs.a -= rhs.a;
    return lhs;
  }

  constexpr
  Color operator-(Color lhs, float rhs) {
    return Color(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a - rhs);
  }

  constexpr
  Color& operator-=(Color& lhs, float rhs) {
    lhs.r -= rhs;
    lhs.g -= rhs;
    lhs.b -= rhs;
    lhs.a -= rhs;
    return lhs;
  }

  constexpr
  Color operator-(float lhs, Color rhs) {
    return Color(lhs - rhs.r, lhs - rhs.g, lhs - rhs.b, lhs - rhs.a);
  }

  /*
   * Multiplication
   */

  constexpr
  Color operator*(Color lhs, Color rhs) {
    return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
  }

  constexpr
  Color& operator*=(Color& lhs, Color rhs) {
    lhs.r *= rhs.r;
    lhs.g *= rhs.g;
    lhs.b *= rhs.b;
    lhs.a *= rhs.a;
    return lhs;
  }

  constexpr
  Color operator*(Color lhs, float rhs) {
    return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
  }

  constexpr
  Color& operator*=(Color& lhs, float rhs) {
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    lhs.a *= rhs;
    return lhs;
  }

  constexpr
  Color operator*(float lhs, Color rhs) {
    return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
  }

  /*
   * Division
   */


  constexpr
  Color operator/(Color lhs, Color rhs) {
    return Color(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
  }

  constexpr
  Color& operator/=(Color& lhs, Color rhs) {
    lhs.r /= rhs.r;
    lhs.g /= rhs.g;
    lhs.b /= rhs.b;
    lhs.a /= rhs.a;
    return lhs;
  }

  constexpr
  Color operator/(Color lhs, float rhs) {
    return Color(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
  }

  constexpr
  Color& operator/=(Color& lhs, float rhs) {
    lhs.r /= rhs;
    lhs.g /= rhs;
    lhs.b /= rhs;
    lhs.a /= rhs;
    return lhs;
  }

  constexpr
  Color operator/(float lhs, Color rhs) {
    return Color(lhs / rhs.r, lhs / rhs.g, lhs / rhs.b, lhs / rhs.a);
  }

  GF_CORE_API Color lighter(Color color, float percent = 0.5f);
  GF_CORE_API Color darker(Color color, float percent = 0.5f);

  constexpr
  Color opaque(float value = 0.5f) {
    return { 1.0f, 1.0f, 1.0f, value };
  }

  constexpr
  Color gray(float value = 0.5f) {
    return { value, value, value, 1.0f };
  }

  constexpr inline Color Black(0.0f, 0.0f, 0.0f);
  constexpr inline Color White(1.0f, 1.0f, 1.0f);
  constexpr inline Color Gray = gray();

  constexpr inline Color Red(1.0f, 0.0f, 0.0f);
  constexpr inline Color Green(0.0f, 1.0f, 0.0f);
  constexpr inline Color Blue(0.0f, 0.0f, 1.0f);
  constexpr inline Color Cyan(0.0f, 1.0f, 1.0f);
  constexpr inline Color Magenta(1.0f, 0.0f, 1.0f);
  constexpr inline Color Yellow(1.0f, 1.0f, 0.0f);

  constexpr inline Color Orange(1.0f, 0.5f, 0.0f);
  constexpr inline Color Rose(1.0f, 0.0f, 0.5f);
  constexpr inline Color Chartreuse(0.5f, 1.0f, 0.0f);
  constexpr inline Color Spring(0.0f, 1.0f, 0.5f);
  constexpr inline Color Violet(0.5f, 0.0f, 1.0f);
  constexpr inline Color Azure(0.0f, 0.5f, 1.0f);

  constexpr inline Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);

}

#endif // GF_COLOR_H
