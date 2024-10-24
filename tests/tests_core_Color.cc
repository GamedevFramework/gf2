#include <type_traits>

#include <gf2/core/Color.h>

#include "gtest/gtest.h"

TEST(ColorTest, DefaultConstructor) {
  gf::Color color0;

  EXPECT_FLOAT_EQ(color0.r, 0.0f);
  EXPECT_FLOAT_EQ(color0.g, 0.0f);
  EXPECT_FLOAT_EQ(color0.b, 0.0f);
  EXPECT_FLOAT_EQ(color0.a, 0.0f);

  constexpr gf::Color Color0;
  static_assert(Color0.r == 0.0f);
  static_assert(Color0.g == 0.0f);
  static_assert(Color0.b == 0.0f);
  static_assert(Color0.a == 0.0f);
}

TEST(ColorTest, Constructor) {
  gf::Color color0(0.0f, 0.5f, 1.0f);

  EXPECT_FLOAT_EQ(color0.r, 0.0f);
  EXPECT_FLOAT_EQ(color0.g, 0.5f);
  EXPECT_FLOAT_EQ(color0.b, 1.0f);
  EXPECT_FLOAT_EQ(color0.a, 1.0f);

  gf::Color color1(0.0f, 0.5f, 1.0f, 0.5f);

  EXPECT_FLOAT_EQ(color1.r, 0.0f);
  EXPECT_FLOAT_EQ(color1.g, 0.5f);
  EXPECT_FLOAT_EQ(color1.b, 1.0f);
  EXPECT_FLOAT_EQ(color1.a, 0.5f);

  constexpr gf::Color Color0(0.0f, 0.5f, 1.0f);
  static_assert(Color0.r == 0.0f);
  static_assert(Color0.g == 0.5f);
  static_assert(Color0.b == 1.0f);
  static_assert(Color0.a == 1.0f);

  constexpr gf::Color Color1(0.0f, 0.5f, 1.0f, 0.5f);
  static_assert(Color1.r == 0.0f);
  static_assert(Color1.g == 0.5f);
  static_assert(Color1.b == 1.0f);
  static_assert(Color1.a == 0.5f);
}

TEST(ColorTest, Rgba) {
  gf::Color color0(0x00FF00);

  EXPECT_FLOAT_EQ(color0.r, 0.0f);
  EXPECT_FLOAT_EQ(color0.g, 1.0f);
  EXPECT_FLOAT_EQ(color0.b, 0.0f);
  EXPECT_FLOAT_EQ(color0.a, 1.0f);

  gf::Color color1(0x00FF00, 0x00);

  EXPECT_FLOAT_EQ(color1.r, 0.0f);
  EXPECT_FLOAT_EQ(color1.g, 1.0f);
  EXPECT_FLOAT_EQ(color1.b, 0.0f);
  EXPECT_FLOAT_EQ(color1.a, 0.0f);

  constexpr gf::Color Color0(0x00FF00);
  static_assert(Color0.r == 0.0f);
  static_assert(Color0.g == 1.0f);
  static_assert(Color0.b == 0.0f);
  static_assert(Color0.a == 1.0f);

  constexpr gf::Color Color1(0x00FF00, 0x00);
  static_assert(Color1.r == 0.0f);
  static_assert(Color1.g == 1.0f);
  static_assert(Color1.b == 0.0f);
  static_assert(Color1.a == 0.0f);
}

TEST(ColorTest, Equality) {
  gf::Color color0(0x123456, 0x78);
  gf::Color color1(0x123456, 0x78);
  gf::Color color2(0x876543, 0x21);

  EXPECT_EQ(color0, color1);
  EXPECT_NE(color0, color2);

  constexpr gf::Color Color0(0x123456, 0x78);
  constexpr gf::Color Color1(0x123456, 0x78);
  constexpr gf::Color Color2(0x876543, 0x21);
  static_assert(Color0 == Color1);
  static_assert(Color0 != Color2);
}

TEST(ColorTest, Addition) {
  gf::Color color0(0.0f, 0.5f, 1.0f, 1.0f);
  gf::Color color1(1.0f, 0.5f, 0.0f, 0.0f);

  EXPECT_EQ(color0 + color1, gf::Color(1.0f, 1.0f, 1.0f, 1.0f));
  EXPECT_EQ(color0 + 0.5f, gf::Color(0.5f, 1.0f, 1.5f, 1.5f));
  EXPECT_EQ(0.5f + color1, gf::Color(1.5f, 1.0f, 0.5f, 0.5f));

  color0 += color1;
  EXPECT_EQ(color0, gf::Color(1.0f, 1.0f, 1.0f, 1.0f));

  color1 += 0.5f;
  EXPECT_EQ(color1, gf::Color(1.5f, 1.0f, 0.5f, 0.5f));

  constexpr gf::Color Color0(0.0f, 0.5f, 1.0f, 1.0f);
  constexpr gf::Color Color1(1.0f, 0.5f, 0.0f, 0.0f);
  static_assert(Color0 + Color1 == gf::Color(1.0f, 1.0f, 1.0f, 1.0f));
  static_assert(Color0 + 0.5f == gf::Color(0.5f, 1.0f, 1.5f, 1.5f));
  static_assert(0.5f + Color1 == gf::Color(1.5f, 1.0f, 0.5f, 0.5f));
}

TEST(ColorTest, Substraction) {
  gf::Color color0(1.0f, 0.5f, 1.0f, 1.0f);
  gf::Color color1(0.5f, 0.5f, 0.0f, 0.0f);

  EXPECT_EQ(color0 - color1, gf::Color(0.5f, 0.0f, 1.0f, 1.0f));
  EXPECT_EQ(color0 - 0.5f, gf::Color(0.5f, 0.0f, 0.5f, 0.5f));
  EXPECT_EQ(0.5f - color1, gf::Color(0.0f, 0.0f, 0.5f, 0.5f));

  color0 -= color1;
  EXPECT_EQ(color0, gf::Color(0.5f, 0.0f, 1.0f, 1.0f));

  color1 -= 0.5f;
  EXPECT_EQ(color1, gf::Color(0.0f, 0.0f, -0.5f, -0.5f));

  constexpr gf::Color Color0(1.0f, 0.5f, 1.0f, 1.0f);
  constexpr gf::Color Color1(0.5f, 0.5f, 0.0f, 0.0f);
  static_assert(Color0 - Color1 == gf::Color(0.5f, 0.0f, 1.0f, 1.0f));
  static_assert(Color0 - 0.5f == gf::Color(0.5f, 0.0f, 0.5f, 0.5f));
  static_assert(0.5f - Color1 == gf::Color(0.0f, 0.0f, 0.5f, 0.5f));
}

TEST(ColorTest, Multiplication) {
  gf::Color color0(1.0f, 1.0f, 0.5f, 0.5f);
  gf::Color color1(1.0f, 0.5f, 0.5f, 1.0f);

  EXPECT_EQ(color0 * color1, gf::Color(1.0f, 0.5f, 0.25f, 0.5f));
  EXPECT_EQ(color0 * 0.5f, gf::Color(0.5f, 0.5f, 0.25f, 0.25f));
  EXPECT_EQ(0.5f * color1, gf::Color(0.5f, 0.25f, 0.25f, 0.5f));

  color0 *= color1;
  EXPECT_EQ(color0, gf::Color(1.0f, 0.5f, 0.25f, 0.5f));

  color1 *= 0.5f;
  EXPECT_EQ(color1, gf::Color(0.5f, 0.25f, 0.25f, 0.5f));

  constexpr gf::Color Color0(1.0f, 1.0f, 0.5f, 0.5f);
  constexpr gf::Color Color1(1.0f, 0.5f, 0.5f, 1.0f);
  static_assert(Color0 * Color1 == gf::Color(1.0f, 0.5f, 0.25f, 0.5f));
  static_assert(Color0 * 0.5f == gf::Color(0.5f, 0.5f, 0.25f, 0.25f));
  static_assert(0.5f * Color1 == gf::Color(0.5f, 0.25f, 0.25f, 0.5f));
}

TEST(ColorTest, Division) {
  gf::Color color0(1.0f, 0.5f, 0.5f, 1.0f);
  gf::Color color1(1.0f, 1.0f, 0.5f, 0.5f);

  EXPECT_EQ(color0 / color1, gf::Color(1.0f, 0.5f, 1.0f, 2.0f));
  EXPECT_EQ(color0 / 0.5f, gf::Color(2.0f, 1.0f, 1.0f, 2.0f));
  EXPECT_EQ(0.5f / color1, gf::Color(0.5f, 0.5f, 1.0f, 1.0f));

  color0 /= color1;
  EXPECT_EQ(color0, gf::Color(1.0f, 0.5f, 1.0f, 2.0f));

  color1 /= 0.5f;
  EXPECT_EQ(color1, gf::Color(2.0f, 2.0f, 1.0f, 1.0f));

  constexpr gf::Color Color0(1.0f, 0.5f, 0.5f, 1.0f);
  constexpr gf::Color Color1(1.0f, 1.0f, 0.5f, 0.5f);
  static_assert(Color0 / Color1 == gf::Color(1.0f, 0.5f, 1.0f, 2.0f));
  static_assert(Color0 / 0.5f == gf::Color(2.0f, 1.0f, 1.0f, 2.0f));
  static_assert(0.5f / Color1 == gf::Color(0.5f, 0.5f, 1.0f, 1.0f));
}

TEST(ColorTest, Opaque) {
  for (auto value : { 0.1f, 0.5f, 0.9f }) {
    gf::Color color = gf::opaque(value);
    EXPECT_EQ(color.r, 1.0f);
    EXPECT_EQ(color.g, 1.0f);
    EXPECT_EQ(color.b, 1.0f);
    EXPECT_EQ(color.a, value);
  }

  gf::Color color = gf::opaque();
  EXPECT_EQ(color.a, 0.5f);

  constexpr gf::Color Color = gf::opaque(0.3f);
  static_assert(Color.r == 1.0f);
  static_assert(Color.g == 1.0f);
  static_assert(Color.b == 1.0f);
  static_assert(Color.a == 0.3f);
}

TEST(ColorTest, ToRgba32) {
  gf::Color color0(0x123456, 0x78);
  auto rgba = color0.to_rgba32();
  EXPECT_EQ(rgba[0], 0x12);
  EXPECT_EQ(rgba[1], 0x34);
  EXPECT_EQ(rgba[2], 0x56);
  EXPECT_EQ(rgba[3], 0x78);

  constexpr gf::Color Color0(0x123456, 0x78);
  constexpr auto Rgba = Color0.to_rgba32();
  static_assert(Rgba[0] == 0x12);
  static_assert(Rgba[1] == 0x34);
  static_assert(Rgba[2] == 0x56);
  static_assert(Rgba[3] == 0x78);
}

TEST(ColorTest, ToHex) {
  gf::Color color0(0x123456, 0x78);
  auto hex = color0.to_hex();
  EXPECT_EQ(hex, 0x12345678);

  constexpr gf::Color Color0(0x123456, 0x78);
  constexpr auto Hex = Color0.to_hex();
  static_assert(Hex == 0x12345678);
}
