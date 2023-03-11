#include <gf2/Vec2.h>

#include <type_traits>

#include "gtest/gtest.h"

TEST(Vec2Test, Constructor) {
  gf::Vec2I vec(3, 4);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.u, 3);
  EXPECT_EQ(vec.v, 4);
  EXPECT_EQ(vec.w, 3);
  EXPECT_EQ(vec.h, 4);

  constexpr gf::Vec2I cvec(3, 4);
  static_assert(cvec.x == 3, "Check Vec2");
  static_assert(cvec.y == 4, "Check Vec2");
}

TEST(Vec2Test, Conversion) {
  gf::Vec2F origin(3.4f, 4.2f);
  gf::Vec2I vec = origin;
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.u, 3);
  EXPECT_EQ(vec.v, 4);
  EXPECT_EQ(vec.w, 3);
  EXPECT_EQ(vec.h, 4);
}

TEST(Vec2Test, Vec) {
  auto vec = gf::vec(3, 4);
  static_assert(std::is_same_v<decltype(vec), gf::Vec2I>, "Check Vec2");

  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);

  constexpr auto cvec = gf::vec(3, 4);
  static_assert(std::is_same_v<decltype(cvec), const gf::Vec2I>, "Check Vec2");
  static_assert(cvec.x == 3, "Check Vec2");
  static_assert(cvec.y == 4, "Check Vec2");
}

TEST(Vec2Test, Equality) {
  gf::Vec2I vec0(3, 4);
  gf::Vec2I vec1(3, 4);
  gf::Vec2I vec2(4, 5);

  EXPECT_EQ(vec0, vec1);
  EXPECT_NE(vec0, vec2);

  constexpr gf::Vec2I cvec0(3, 4);
  constexpr gf::Vec2I cvec1(3, 4);
  constexpr gf::Vec2I cvec2(4, 5);
  static_assert(cvec0 == cvec1, "Check Vec2");
  static_assert(cvec0 != cvec2, "Check Vec2");
}

TEST(Vec2Test, Opposite) {
  gf::Vec2I vec0(3, 4);

  EXPECT_EQ(-vec0, gf::vec(-3, -4));

  constexpr gf::Vec2I cvec0(3, 4);
  static_assert(-cvec0 == gf::vec(-3, -4), "Check Vec2");
}

TEST(Vec2Test, Addition) {
  gf::Vec2I vec0(6, 13);
  gf::Vec2I vec1(3, 4);

  EXPECT_EQ(vec0 + vec1, gf::vec(9, 17));
  EXPECT_EQ(vec0 + 5, gf::vec(11, 18));
  EXPECT_EQ(12 + vec0, gf::vec(18, 25));

  vec0 += vec1;
  EXPECT_EQ(vec0, gf::vec(9, 17));

  vec0 += 5;
  EXPECT_EQ(vec0, gf::vec(14, 22));

  constexpr gf::Vec2I cvec0(6, 13);
  constexpr gf::Vec2I cvec1(3, 4);
  static_assert(cvec0 + cvec1 == gf::vec(9, 17), "Check Vec2");
  static_assert(cvec0 + 5 == gf::vec(11, 18), "Check Vec2");
  static_assert(12 + cvec0 == gf::vec(18, 25), "Check Vec2");
}

TEST(Vec2Test, Substraction) {
  gf::Vec2I vec0(6, 13);
  gf::Vec2I vec1(3, 4);

  EXPECT_EQ(vec0 - vec1, gf::vec(3, 9));
  EXPECT_EQ(vec0 - 5, gf::vec(1, 8));
  EXPECT_EQ(12 - vec0, gf::vec(6, -1));

  vec0 -= vec1;
  EXPECT_EQ(vec0, gf::vec(3, 9));

  vec0 -= 5;
  EXPECT_EQ(vec0, gf::vec(-2, 4));

  constexpr gf::Vec2I cvec0(6, 13);
  constexpr gf::Vec2I cvec1(3, 4);
  static_assert(cvec0 - cvec1 == gf::vec(3, 9), "Check Vec2");
  static_assert(cvec0 - 5 == gf::vec(1, 8), "Check Vec2");
  static_assert(12 - cvec0 == gf::vec(6, -1), "Check Vec2");
}

TEST(Vec2Test, Multiplication) {
  gf::Vec2I vec0(6, 13);
  gf::Vec2I vec1(3, 4);

  EXPECT_EQ(vec0 * vec1, gf::vec(18, 52));
  EXPECT_EQ(vec0 * 5, gf::vec(30, 65));
  EXPECT_EQ(12 * vec0, gf::vec(72, 156));

  vec0 *= vec1;
  EXPECT_EQ(vec0, gf::vec(18, 52));

  vec0 *= 5;
  EXPECT_EQ(vec0, gf::vec(90, 260));

  constexpr gf::Vec2I cvec0(6, 13);
  constexpr gf::Vec2I cvec1(3, 4);
  static_assert(cvec0 * cvec1 == gf::vec(18, 52), "Check Vec2");
  static_assert(cvec0 * 5 == gf::vec(30, 65), "Check Vec2");
  static_assert(12 * cvec0 == gf::vec(72, 156), "Check Vec2");
}

TEST(Vec2Test, Division) {
  gf::Vec2I vec0(6, 13);
  gf::Vec2I vec1(3, 4);

  EXPECT_EQ(vec0 / vec1, gf::vec(2, 3));
  EXPECT_EQ(vec0 / 5, gf::vec(1, 2));
  EXPECT_EQ(12 / vec0, gf::vec(2, 0));

  vec0 /= vec1;
  EXPECT_EQ(vec0, gf::vec(2, 3));

  vec0 /= 5;
  EXPECT_EQ(vec0, gf::vec(0, 0));

  constexpr gf::Vec2I cvec0(6, 13);
  constexpr gf::Vec2I cvec1(3, 4);
  static_assert(cvec0 / cvec1 == gf::vec(2, 3), "Check Vec2");
  static_assert(cvec0 / 5 == gf::vec(1, 2), "Check Vec2");
  static_assert(12 / cvec0 == gf::vec(2, 0), "Check Vec2");
}

TEST(Vec2Test, Dot) {
  gf::Vec2I vec0(3, 4);
  gf::Vec2I vec1(5, 6);

  EXPECT_EQ(gf::dot(vec0, vec1), 39);

  constexpr gf::Vec2I cvec0(3, 4);
  constexpr gf::Vec2I cvec1(5, 6);
  static_assert(gf::dot(cvec0, cvec1) == 39, "Check Vec2");
}

TEST(Vec2Test, ManhattanLength) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::manhattan_length(vec0), 7);

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::manhattan_length(cvec0) == 7, "Check Vec2");
}

TEST(Vec2Test, ManhattanDistance) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);

  EXPECT_EQ(gf::manhattan_distance(vec0, vec1), 14);

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  static_assert(gf::manhattan_distance(cvec0, cvec1) == 14, "Check Vec2");
}

TEST(Vec2Test, SquareLength) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::square_length(vec0), 25);

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::square_length(cvec0) == 25, "Check Vec2");
}

TEST(Vec2Test, SquareDistance) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);

  EXPECT_EQ(gf::square_distance(vec0, vec1), 100);

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  static_assert(gf::square_distance(cvec0, cvec1) == 100, "Check Vec2");
}

TEST(Vec2Test, EuclideanLength) {
  gf::Vec2F vec0(-3.0f, 4.0f);

  EXPECT_FLOAT_EQ(gf::euclidean_length(vec0), 5.0f);
}

TEST(Vec2Test, EuclideanDistance) {
  gf::Vec2F vec0(-3.0f, 4.0f);
  gf::Vec2F vec1(5.0f, -2.0f);

  EXPECT_EQ(gf::euclidean_distance(vec0, vec1), 10.0f);
}

TEST(Vec2Test, CheyshevLength) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::chebyshev_length(vec0), 4);

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::chebyshev_length(cvec0) == 4, "Check Vec2");
}

TEST(Vec2Test, CheyshevDistance) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);

  EXPECT_EQ(gf::chebyshev_distance(vec0, vec1), 8);

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  static_assert(gf::chebyshev_distance(cvec0, cvec1) == 8, "Check Vec2");
}

TEST(Vec2Test, NaturalLength) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::natural_length(vec0), 32);

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::natural_length(cvec0) == 32);
}

TEST(Vec2Test, NaturalDistance) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);

  EXPECT_EQ(gf::natural_distance(vec0, vec1), 114);

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  static_assert(gf::natural_distance(cvec0, cvec1) == 114);
}

TEST(Vec2Test, Normalize) {
  gf::Vec2F vec0(5.0f, 5.0f);

  EXPECT_EQ(gf::normalize(vec0), gf::vec(gf::InvSqrt2, gf::InvSqrt2));
}

TEST(Vec2Test, ProjX) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::projx(vec0), gf::vec(-3, 0));

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::projx(cvec0) == gf::vec(-3, 0), "Check Vec2");
}

TEST(Vec2Test, ProjY) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::projy(vec0), gf::vec(0, 4));

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::projy(cvec0) == gf::vec(0, 4), "Check Vec2");
}

TEST(Vec2Test, DirX) {
  EXPECT_EQ(gf::dirx(3), gf::vec(3, 0));
  static_assert(gf::dirx(3) == gf::vec(3, 0), "Check Vec2");
}

TEST(Vec2Test, DirY) {
  EXPECT_EQ(gf::diry(3), gf::vec(0, 3));
  static_assert(gf::diry(3) == gf::vec(0, 3), "Check Vec2");
}

TEST(Vec2Test, Unit) {
  auto vec0 = gf::unit(0.0f);

  EXPECT_TRUE(gf::almost_equals(vec0.x, 1.0f));
  EXPECT_TRUE(gf::almost_equals(vec0.y, 0.0f));

  auto vec1 = gf::unit(gf::Pi2);

  EXPECT_TRUE(gf::almost_equals(vec1.x, 0.0f));
  EXPECT_TRUE(gf::almost_equals(vec1.y, 1.0f));

  auto vec2 = gf::unit(gf::Pi);

  EXPECT_TRUE(gf::almost_equals(vec2.x, -1.0f));
  EXPECT_TRUE(gf::almost_equals(vec2.y, 0.0f));
}

TEST(Vec2Test, Angle) {
  auto a0 = gf::angle(gf::vec(1.0f, 0.0f));
  EXPECT_FLOAT_EQ(a0, 0.0f);

  auto a1 = gf::angle(gf::vec(0.0f, 1.0f));
  EXPECT_FLOAT_EQ(a1, gf::Pi2);

  auto a2 = gf::angle(gf::vec(-1.0f, 0.0f));
  EXPECT_FLOAT_EQ(a2, gf::Pi);

  auto a3 = gf::angle(gf::vec(1.0f, 1.0f));
  EXPECT_FLOAT_EQ(a3, gf::Pi4);
}

TEST(Vec2Test, Perp) {
  gf::Vec2I vec0(-3, 4);

  EXPECT_EQ(gf::perp(vec0), gf::vec(-4, -3));

  constexpr gf::Vec2I cvec0(-3, 4);
  static_assert(gf::perp(cvec0) == gf::vec(-4, -3), "Check Vec2");
}

TEST(Vec2Test, TripleProduct) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);
  gf::Vec2I vec2(-1, 6);

  EXPECT_EQ(gf::triple_product(vec0, vec1, vec2), gf::vec(112, 84));

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  constexpr gf::Vec2I cvec2(-1, 6);
  static_assert(gf::triple_product(cvec0, cvec1, cvec2) == gf::vec(112, 84), "Check Vec2");
}

TEST(Vec2Test, InverseTripleProduct) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);
  gf::Vec2I vec2(-1, 6);

  EXPECT_EQ(gf::inverse_triple_product(vec0, vec1, vec2), gf::vec(84, 14));

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  constexpr gf::Vec2I cvec2(-1, 6);
  static_assert(gf::inverse_triple_product(cvec0, cvec1, cvec2) == gf::vec(84, 14), "Check Vec2");
}

TEST(Vec2Test, Cross) {
  gf::Vec2I vec0(-3, 4);
  gf::Vec2I vec1(5, -2);

  EXPECT_EQ(gf::cross(vec0, vec1), -14);

  constexpr gf::Vec2I cvec0(-3, 4);
  constexpr gf::Vec2I cvec1(5, -2);
  static_assert(gf::cross(cvec0, cvec1) == -14, "Check Vec2");
}

TEST(Vec2Test, AngleTo) {
  gf::Vec2F vec0(1.0, 1.0);
  gf::Vec2F vec1(-1.0, 1.0);

  EXPECT_FLOAT_EQ(gf::angle_to(vec0, vec1), gf::Pi2);
}

TEST(Vec2Test, ProjectOnto) {
  gf::Vec2I vec0(3, 3);
  gf::Vec2I vec1(1, 0);

  EXPECT_EQ(gf::project_onto(vec0, vec1), gf::vec(3, 0));

  constexpr gf::Vec2I cvec0(3, 3);
  constexpr gf::Vec2I cvec1(1, 0);
  static_assert(gf::project_onto(cvec0, cvec1) == gf::vec(3, 0), "Check Vec2");
}
