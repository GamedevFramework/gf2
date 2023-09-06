#include <type_traits>

#include <gf2/core/Color.h>

#include "gtest/gtest.h"

TEST(ColorTest, DefaultConstructor) {
  gf::Color color0;

  EXPECT_FLOAT_EQ(color0.r, 0.0f);
  EXPECT_FLOAT_EQ(color0.g, 0.0f);
  EXPECT_FLOAT_EQ(color0.b, 0.0f);
  EXPECT_FLOAT_EQ(color0.a, 0.0f);

  constexpr gf::Color ccolor0;
  static_assert(ccolor0.r == 0.0f, "Check Color");
  static_assert(ccolor0.g == 0.0f, "Check Color");
  static_assert(ccolor0.b == 0.0f, "Check Color");
  static_assert(ccolor0.a == 0.0f, "Check Color");
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

  constexpr gf::Color ccolor0(0.0f, 0.5f, 1.0f);
  static_assert(ccolor0.r == 0.0f, "Check Color");
  static_assert(ccolor0.g == 0.5f, "Check Color");
  static_assert(ccolor0.b == 1.0f, "Check Color");
  static_assert(ccolor0.a == 1.0f, "Check Color");

  constexpr gf::Color ccolor1(0.0f, 0.5f, 1.0f, 0.5f);
  static_assert(ccolor1.r == 0.0f, "Check Color");
  static_assert(ccolor1.g == 0.5f, "Check Color");
  static_assert(ccolor1.b == 1.0f, "Check Color");
  static_assert(ccolor1.a == 0.5f, "Check Color");
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

  constexpr gf::Color ccolor0(0x00FF00);
  static_assert(ccolor0.r == 0.0f, "Check Color");
  static_assert(ccolor0.g == 1.0f, "Check Color");
  static_assert(ccolor0.b == 0.0f, "Check Color");
  static_assert(ccolor0.a == 1.0f, "Check Color");

  constexpr gf::Color ccolor1(0x00FF00, 0x00);
  static_assert(ccolor1.r == 0.0f, "Check Color");
  static_assert(ccolor1.g == 1.0f, "Check Color");
  static_assert(ccolor1.b == 0.0f, "Check Color");
  static_assert(ccolor1.a == 0.0f, "Check Color");
}

TEST(ColorTest, Equality) {
  gf::Color color0(0x12345678);
  gf::Color color1(0x12345678);
  gf::Color color2(0x87654321);

  EXPECT_EQ(color0, color1);
  EXPECT_NE(color0, color2);

  constexpr gf::Color ccolor0(0x12345678);
  constexpr gf::Color ccolor1(0x12345678);
  constexpr gf::Color ccolor2(0x87654321);
  static_assert(ccolor0 == ccolor1, "Check Color");
  static_assert(ccolor0 != ccolor2, "Check Color");
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

  constexpr gf::Color ccolor0(0.0f, 0.5f, 1.0f, 1.0f);
  constexpr gf::Color ccolor1(1.0f, 0.5f, 0.0f, 0.0f);
  static_assert(ccolor0 + ccolor1 == gf::Color(1.0f, 1.0f, 1.0f, 1.0f), "Check Color");
  static_assert(ccolor0 + 0.5f == gf::Color(0.5f, 1.0f, 1.5f, 1.5f), "Check Color");
  static_assert(0.5f + ccolor1 == gf::Color(1.5f, 1.0f, 0.5f, 0.5f), "Check Color");
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

  constexpr gf::Color ccolor0(1.0f, 0.5f, 1.0f, 1.0f);
  constexpr gf::Color ccolor1(0.5f, 0.5f, 0.0f, 0.0f);
  static_assert(ccolor0 - ccolor1 == gf::Color(0.5f, 0.0f, 1.0f, 1.0f), "Check Color");
  static_assert(ccolor0 - 0.5f == gf::Color(0.5f, 0.0f, 0.5f, 0.5f), "Check Color");
  static_assert(0.5f - ccolor1 == gf::Color(0.0f, 0.0f, 0.5f, 0.5f), "Check Color");
}

TEST(ColorTest, Multiplication) {
  gf::Color color0(1.0f, 1.0f, 0.5f, 0.5f);
  gf::Color color1(1.0f, 0.5f, 0.5f, 1.0f);

  EXPECT_EQ(color0* color1, gf::Color(1.0f, 0.5f, 0.25f, 0.5f));
  EXPECT_EQ(color0 * 0.5f, gf::Color(0.5f, 0.5f, 0.25f, 0.25f));
  EXPECT_EQ(0.5f * color1, gf::Color(0.5f, 0.25f, 0.25f, 0.5f));

  color0 *= color1;
  EXPECT_EQ(color0, gf::Color(1.0f, 0.5f, 0.25f, 0.5f));

  color1 *= 0.5f;
  EXPECT_EQ(color1, gf::Color(0.5f, 0.25f, 0.25f, 0.5f));

  constexpr gf::Color ccolor0(1.0f, 1.0f, 0.5f, 0.5f);
  constexpr gf::Color ccolor1(1.0f, 0.5f, 0.5f, 1.0f);
  static_assert(ccolor0 * ccolor1 == gf::Color(1.0f, 0.5f, 0.25f, 0.5f), "Check Color");
  static_assert(ccolor0 * 0.5f == gf::Color(0.5f, 0.5f, 0.25f, 0.25f), "Check Color");
  static_assert(0.5f * ccolor1 == gf::Color(0.5f, 0.25f, 0.25f, 0.5f), "Check Color");
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

  constexpr gf::Color ccolor0(1.0f, 0.5f, 0.5f, 1.0f);
  constexpr gf::Color ccolor1(1.0f, 1.0f, 0.5f, 0.5f);
  static_assert(ccolor0 / ccolor1 == gf::Color(1.0f, 0.5f, 1.0f, 2.0f), "Check Color");
  static_assert(ccolor0 / 0.5f == gf::Color(2.0f, 1.0f, 1.0f, 2.0f), "Check Color");
  static_assert(0.5f / ccolor1 == gf::Color(0.5f, 0.5f, 1.0f, 1.0f), "Check Color");
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

  constexpr gf::Color ccolor = gf::opaque(0.3f);
  static_assert(ccolor.r == 1.0f, "Check Color");
  static_assert(ccolor.g == 1.0f, "Check Color");
  static_assert(ccolor.b == 1.0f, "Check Color");
  static_assert(ccolor.a == 0.3f, "Check Color");
}
