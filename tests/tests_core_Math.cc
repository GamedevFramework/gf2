#include <type_traits>

#include <gf2/Math.h>

#include "gtest/gtest.h"

TEST(MathTest, AlmostEqualsDouble) {
  EXPECT_TRUE(gf::almost_equals(1.0 + 2.0, 3.0));
  EXPECT_TRUE(gf::almost_equals(42.0, 42.00000000000001));
  EXPECT_TRUE(gf::almost_equals(0.0, std::numeric_limits<double>::min()));
  EXPECT_TRUE(gf::almost_equals(0.0, std::numeric_limits<double>::epsilon()));

  EXPECT_FALSE(gf::almost_equals(42.0, 42.0000000000001));

  constexpr bool should_be_true = gf::almost_equals(42.0, 42.00000000000001);
  static_assert(should_be_true, "Check almost_equals");
  EXPECT_TRUE(should_be_true);

  constexpr bool should_be_false = gf::almost_equals(42.0, 42.0000000000001);
  static_assert(!should_be_false, "Check almost_equals");
  EXPECT_FALSE(should_be_false);
}

TEST(MathTest, AlmostEqualsFloat) {
  EXPECT_TRUE(gf::almost_equals(1.0f + 2.0f, 3.0f));
  EXPECT_TRUE(gf::almost_equals(42.0f, 42.000001f));
  EXPECT_TRUE(gf::almost_equals(0.0f, std::numeric_limits<float>::min()));
  EXPECT_TRUE(gf::almost_equals(0.0f, std::numeric_limits<float>::epsilon()));

  EXPECT_FALSE(gf::almost_equals(42.0f, 42.00001f));

  constexpr bool should_be_true = gf::almost_equals(42.0f, 42.000001f);
  static_assert(should_be_true, "Check almost_equals");
  EXPECT_TRUE(should_be_true);

  constexpr bool should_be_false = gf::almost_equals(42.0f, 42.00001f);
  static_assert(!should_be_false, "Check almost_equals");
  EXPECT_FALSE(should_be_false);
}

TEST(MathTest, DegreesToRadians) {
  EXPECT_FLOAT_EQ(gf::degrees_to_radians(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(gf::degrees_to_radians(45.0f), gf::Pi4);
  EXPECT_FLOAT_EQ(gf::degrees_to_radians(90.0f), gf::Pi2);
  EXPECT_FLOAT_EQ(gf::degrees_to_radians(180.0f), gf::Pi);

  EXPECT_DOUBLE_EQ(gf::degrees_to_radians(0.0), 0.0);
  EXPECT_DOUBLE_EQ(gf::degrees_to_radians(45.0), gf::constants::Pi<double> / 4);
  EXPECT_DOUBLE_EQ(gf::degrees_to_radians(90.0), gf::constants::Pi<double> / 2);
  EXPECT_DOUBLE_EQ(gf::degrees_to_radians(180.0), gf::constants::Pi<double>);

  constexpr auto value_float = gf::degrees_to_radians(0.0f);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_float)>, float>, "Check degrees_to_radians");
  static_assert(value_float == 0.0f, "Check degrees_to_radians");

  constexpr auto value_double = gf::degrees_to_radians(0.0);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_double)>, double>, "Check degrees_to_radians");
  static_assert(value_double == 0.0, "Check degrees_to_radians");
}

TEST(MathTest, RadiansToDegrees) {
  EXPECT_FLOAT_EQ(gf::radians_to_degrees(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(gf::radians_to_degrees(gf::Pi4), 45.0f);
  EXPECT_FLOAT_EQ(gf::radians_to_degrees(gf::Pi2), 90.0f);
  EXPECT_FLOAT_EQ(gf::radians_to_degrees(gf::Pi), 180.0f);

  EXPECT_DOUBLE_EQ(gf::radians_to_degrees(0.0), 0.0);
  EXPECT_DOUBLE_EQ(gf::radians_to_degrees(gf::constants::Pi<double> / 4), 45.0);
  EXPECT_DOUBLE_EQ(gf::radians_to_degrees(gf::constants::Pi<double> / 2), 90.0);
  EXPECT_DOUBLE_EQ(gf::radians_to_degrees(gf::constants::Pi<double>), 180.0);

  constexpr auto value_float = gf::radians_to_degrees(0.0f);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_float)>, float>, "Check radians_to_degrees");
  static_assert(value_float == 0.0f, "Check radians_to_degrees");

  constexpr auto value_double = gf::radians_to_degrees(0.0);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_double)>, double>, "Check radians_to_degrees");
  static_assert(value_double == 0.0, "Check radians_to_degrees");
}

TEST(MathTest, Square) {
  EXPECT_EQ(gf::square(3), 9);
  EXPECT_EQ(gf::square(3.0), 9.0);
  EXPECT_EQ(gf::square(3.0f), 9.0f);

  constexpr auto value_int = gf::square(3);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_int)>, int>, "Check square");
  static_assert(value_int == 9, "Check square");
  EXPECT_EQ(value_int, 9);

  constexpr auto value_double = gf::square(3.0);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_double)>, double>, "Check square");
  static_assert(value_double == 9.0, "Check square");
  EXPECT_EQ(value_double, 9.0);

  constexpr auto value_float = gf::square(3.0f);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_float)>, float>, "Check square");
  static_assert(value_float == 9.0f, "Check square");
  EXPECT_EQ(value_float, 9.0f);
}

TEST(MathTest, Cube) {
  EXPECT_EQ(gf::cube(3), 27);
  EXPECT_EQ(gf::cube(3.0), 27.0);
  EXPECT_EQ(gf::cube(3.0f), 27.0f);

  constexpr auto value_int = gf::cube(3);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_int)>, int>, "Check cube");
  static_assert(value_int == 27, "Check cube");
  EXPECT_EQ(value_int, 27);

  constexpr auto value_double = gf::cube(3.0);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_double)>, double>, "Check cube");
  static_assert(value_double == 27.0, "Check cube");
  EXPECT_EQ(value_double, 27.0);

  constexpr auto value_float = gf::cube(3.0f);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_float)>, float>, "Check cube");
  static_assert(value_float == 27.0f, "Check cube");
  EXPECT_EQ(value_float, 27.0f);
}

TEST(MathTest, AbsDiff) {
  EXPECT_EQ(gf::absdiff(8, 3), 5);
  EXPECT_EQ(gf::absdiff(3, 8), 5);

  EXPECT_EQ(gf::absdiff(8u, 3u), 5u);
  EXPECT_EQ(gf::absdiff(3u, 8u), 5u);

  constexpr auto value_int_pos = gf::absdiff(8, 3);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_int_pos)>, int>, "Check absdiff");
  static_assert(value_int_pos == 5, "Check absdiff");

  constexpr auto value_int_neg = gf::absdiff(3, 8);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_int_neg)>, int>, "Check absdiff");
  static_assert(value_int_neg == 5, "Check absdiff");

  constexpr auto value_unsigned_pos = gf::absdiff(8u, 3u);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_unsigned_pos)>, unsigned>, "Check absdiff");
  static_assert(value_unsigned_pos == 5u, "Check absdiff");

  constexpr auto value_unsigned_neg = gf::absdiff(3u, 8u);
  static_assert(std::is_same_v<std::remove_cv_t<decltype(value_unsigned_neg)>, unsigned>, "Check absdiff");
  static_assert(value_unsigned_neg == 5u, "Check absdiff");
}

TEST(MathTest, Parity) {
  EXPECT_EQ(gf::parity(0), 0);
  EXPECT_EQ(gf::parity(3), 1);
  EXPECT_EQ(gf::parity(4), 0);
  EXPECT_EQ(gf::parity(-3), 1);
  EXPECT_EQ(gf::parity(-4), 0);

  static_assert(gf::parity(0) == 0, "Check parity");
  static_assert(gf::parity(3) == 1, "Check parity");
  static_assert(gf::parity(4) == 0, "Check parity");
  static_assert(gf::parity(-3) == 1, "Check parity");
  static_assert(gf::parity(-4) == 0, "Check parity");
}

TEST(MathTest, DivFloor) {
  EXPECT_EQ(gf::div_floor(+3, +2), +1);
  EXPECT_EQ(gf::div_floor(+3, -2), -2);
  EXPECT_EQ(gf::div_floor(-3, +2), -2);
  EXPECT_EQ(gf::div_floor(-3, -2), +1);

  EXPECT_EQ(gf::div_floor(+4, +2), +2);
  EXPECT_EQ(gf::div_floor(+4, -2), -2);
  EXPECT_EQ(gf::div_floor(-4, +2), -2);
  EXPECT_EQ(gf::div_floor(-4, -2), +2);

  EXPECT_EQ(gf::div_floor(3u, 2u), 1u);
  EXPECT_EQ(gf::div_floor(4u, 2u), 2u);

  static_assert(gf::div_floor(+3, +2) == +1, "Check div_floor");
  static_assert(gf::div_floor(+3, -2) == -2, "Check div_floor");
  static_assert(gf::div_floor(-3, +2) == -2, "Check div_floor");
  static_assert(gf::div_floor(-3, -2) == +1, "Check div_floor");

  static_assert(gf::div_floor(+4, +2) == +2, "Check div_floor");
  static_assert(gf::div_floor(+4, -2) == -2, "Check div_floor");
  static_assert(gf::div_floor(-4, +2) == -2, "Check div_floor");
  static_assert(gf::div_floor(-4, -2) == +2, "Check div_floor");
}

TEST(MathTest, DivCeil) {
  EXPECT_EQ(gf::div_ceil(+3, +2), +2);
  EXPECT_EQ(gf::div_ceil(+3, -2), -1);
  EXPECT_EQ(gf::div_ceil(-3, +2), -1);
  EXPECT_EQ(gf::div_ceil(-3, -2), +2);

  EXPECT_EQ(gf::div_ceil(+4, +2), +2);
  EXPECT_EQ(gf::div_ceil(+4, -2), -2);
  EXPECT_EQ(gf::div_ceil(-4, +2), -2);
  EXPECT_EQ(gf::div_ceil(-4, -2), +2);

  EXPECT_EQ(gf::div_ceil(3u, 2u), 2u);
  EXPECT_EQ(gf::div_ceil(4u, 2u), 2u);

  static_assert(gf::div_ceil(+3, +2) == +2, "Check div_ceil");
  static_assert(gf::div_ceil(+3, -2) == -1, "Check div_ceil");
  static_assert(gf::div_ceil(-3, +2) == -1, "Check div_ceil");
  static_assert(gf::div_ceil(-3, -2) == +2, "Check div_ceil");

  static_assert(gf::div_ceil(+4, +2) == +2, "Check div_ceil");
  static_assert(gf::div_ceil(+4, -2) == -2, "Check div_ceil");
  static_assert(gf::div_ceil(-4, +2) == -2, "Check div_ceil");
  static_assert(gf::div_ceil(-4, -2) == +2, "Check div_ceil");
}
