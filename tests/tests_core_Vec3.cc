#include <type_traits>

#include <gf2/Vec3.h>

#include "gtest/gtest.h"

TEST(Vec3Test, Constructor) {
  gf::Vec3I vec(3, 4, 5);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 5);

  constexpr gf::Vec3I cvec(3, 4, 5);
  static_assert(cvec.x == 3, "Check Vec3");
  static_assert(cvec.y == 4, "Check Vec3");
  static_assert(cvec.z == 5, "Check Vec3");
}

TEST(Vec3Test, Conversion) {
  gf::Vec3F origin(3.4f, 4.2f, 5.7f);
  gf::Vec3I vec = origin;
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 5);
}

TEST(Vec3Test, Vec) {
  auto vec = gf::vec(3, 4, 5);
  static_assert(std::is_same_v<decltype(vec), gf::Vec3I>, "Check Vec3");

  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 5);

  constexpr auto cvec = gf::vec(3, 4, 5);
  static_assert(std::is_same_v<decltype(cvec), const gf::Vec3I>, "Check Vec3");
  static_assert(cvec.x == 3, "Check Vec3");
  static_assert(cvec.y == 4, "Check Vec3");
  static_assert(cvec.z == 5, "Check Vec3");
}

TEST(Vec3Test, Equality) {
  gf::Vec3I vec0(3, 4, 5);
  gf::Vec3I vec1(3, 4, 5);
  gf::Vec3I vec2(4, 5, 6);

  EXPECT_EQ(vec0, vec1);
  EXPECT_NE(vec0, vec2);

  constexpr gf::Vec3I cvec0(3, 4, 5);
  constexpr gf::Vec3I cvec1(3, 4, 5);
  constexpr gf::Vec3I cvec2(4, 5, 6);
  static_assert(cvec0 == cvec1, "Check Vec3");
  static_assert(cvec0 != cvec2, "Check Vec3");
}

TEST(Vec3Test, Opposite) {
  gf::Vec3I vec0(3, 4, 5);

  EXPECT_EQ(-vec0, gf::vec(-3, -4, -5));

  constexpr gf::Vec3I cvec0(3, 4, 5);
  static_assert(-cvec0 == gf::vec(-3, -4, -5), "Check Vec3");
}

TEST(Vec3Test, Addition) {
  gf::Vec3I vec0(6, 13, 21);
  gf::Vec3I vec1(3, 4, 5);

  EXPECT_EQ(vec0 + vec1, gf::vec(9, 17, 26));
  EXPECT_EQ(vec0 + 5, gf::vec(11, 18, 26));
  EXPECT_EQ(12 + vec0, gf::vec(18, 25, 33));

  vec0 += vec1;
  EXPECT_EQ(vec0, gf::vec(9, 17, 26));

  vec0 += 5;
  EXPECT_EQ(vec0, gf::vec(14, 22, 31));

  constexpr gf::Vec3I cvec0(6, 13, 21);
  constexpr gf::Vec3I cvec1(3, 4, 5);
  static_assert(cvec0 + cvec1 == gf::vec(9, 17, 26), "Check Vec3");
  static_assert(cvec0 + 5 == gf::vec(11, 18, 26), "Check Vec3");
  static_assert(12 + cvec0 == gf::vec(18, 25, 33), "Check Vec3");
}

TEST(Vec3Test, Substraction) {
  gf::Vec3I vec0(6, 13, 21);
  gf::Vec3I vec1(3, 4, 5);

  EXPECT_EQ(vec0 - vec1, gf::vec(3, 9, 16));
  EXPECT_EQ(vec0 - 5, gf::vec(1, 8, 16));
  EXPECT_EQ(12 - vec0, gf::vec(6, -1, -9));

  vec0 -= vec1;
  EXPECT_EQ(vec0, gf::vec(3, 9, 16));

  vec0 -= 5;
  EXPECT_EQ(vec0, gf::vec(-2, 4, 11));

  constexpr gf::Vec3I cvec0(6, 13, 21);
  constexpr gf::Vec3I cvec1(3, 4, 5);
  static_assert(cvec0 - cvec1 == gf::vec(3, 9, 16), "Check Vec3");
  static_assert(cvec0 - 5 == gf::vec(1, 8, 16), "Check Vec3");
  static_assert(12 - cvec0 == gf::vec(6, -1, -9), "Check Vec3");
}

TEST(Vec3Test, Multiplication) {
  gf::Vec3I vec0(6, 13, 21);
  gf::Vec3I vec1(3, 4, 5);

  // clang-format off
  EXPECT_EQ(vec0 * vec1, gf::vec(18, 52, 105));
  // clang-format on
  EXPECT_EQ(vec0 * 5, gf::vec(30, 65, 105));
  EXPECT_EQ(12 * vec0, gf::vec(72, 156, 252));

  vec0 *= vec1;
  EXPECT_EQ(vec0, gf::vec(18, 52, 105));

  vec0 *= 5;
  EXPECT_EQ(vec0, gf::vec(90, 260, 525));

  constexpr gf::Vec3I cvec0(6, 13, 21);
  constexpr gf::Vec3I cvec1(3, 4, 5);
  static_assert(cvec0 * cvec1 == gf::vec(18, 52, 105), "Check Vec3");
  static_assert(cvec0 * 5 == gf::vec(30, 65, 105), "Check Vec3");
  static_assert(12 * cvec0 == gf::vec(72, 156, 252), "Check Vec3");
}

TEST(Vec3Test, Division) {
  gf::Vec3I vec0(6, 13, 21);
  gf::Vec3I vec1(3, 4, 5);

  EXPECT_EQ(vec0 / vec1, gf::vec(2, 3, 4));
  EXPECT_EQ(vec0 / 5, gf::vec(1, 2, 4));
  EXPECT_EQ(12 / vec0, gf::vec(2, 0, 0));

  vec0 /= vec1;
  EXPECT_EQ(vec0, gf::vec(2, 3, 4));

  vec0 /= 5;
  EXPECT_EQ(vec0, gf::vec(0, 0, 0));

  constexpr gf::Vec3I cvec0(6, 13, 21);
  constexpr gf::Vec3I cvec1(3, 4, 5);
  static_assert(cvec0 / cvec1 == gf::vec(2, 3, 4), "Check Vec3");
  static_assert(cvec0 / 5 == gf::vec(1, 2, 4), "Check Vec3");
  static_assert(12 / cvec0 == gf::vec(2, 0, 0), "Check Vec3");
}

TEST(Vec3Test, Dot) {
  gf::Vec3I vec0(3, 4, 5);
  gf::Vec3I vec1(5, 6, 7);

  EXPECT_EQ(gf::dot(vec0, vec1), 74);

  constexpr gf::Vec3I cvec0(3, 4, 5);
  constexpr gf::Vec3I cvec1(5, 6, 7);
  static_assert(gf::dot(cvec0, cvec1) == 74, "Check Vec3");
}

TEST(Vec3Test, ManhattanLength) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::manhattan_length(vec0), 12);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::manhattan_length(cvec0) == 12, "Check Vec3");
}

TEST(Vec3Test, ManhattanDistance) {
  gf::Vec3I vec0(-3, 4, -5);
  gf::Vec3I vec1(5, -2, 1);

  EXPECT_EQ(gf::manhattan_distance(vec0, vec1), 20);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  constexpr gf::Vec3I cvec1(5, -2, 1);
  static_assert(gf::manhattan_distance(cvec0, cvec1) == 20, "Check Vec3");
}

TEST(Vec3Test, SquareLength) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::square_length(vec0), 50);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::square_length(cvec0) == 50, "Check Vec3");
}

TEST(Vec3Test, SquareDistance) {
  gf::Vec3I vec0(-3, 4, -5);
  gf::Vec3I vec1(5, -2, 1);

  EXPECT_EQ(gf::square_distance(vec0, vec1), 136);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  constexpr gf::Vec3I cvec1(5, -2, 1);
  static_assert(gf::square_distance(cvec0, cvec1) == 136, "Check Vec3");
}

TEST(Vec3Test, EuclideanLength) {
  gf::Vec3F vec0(-2.0f, 3.0f, -6.0f);

  EXPECT_FLOAT_EQ(gf::euclidean_length(vec0), 7.0f);
}

TEST(Vec3Test, EuclideanDistance) {
  gf::Vec3F vec0(-2.0f, 3.0f, -6.0f);
  gf::Vec3F vec1(2.0f, -1.0f, 1.0f);

  EXPECT_FLOAT_EQ(gf::euclidean_distance(vec0, vec1), 9.0f);
}

TEST(Vec3Test, CheyshevLength) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::chebyshev_length(vec0), 5);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::chebyshev_length(cvec0) == 5, "Check Vec3");
}

TEST(Vec3Test, CheyshevDistance) {
  gf::Vec3I vec0(-3, 4, -5);
  gf::Vec3I vec1(5, -2, 1);

  EXPECT_EQ(gf::chebyshev_distance(vec0, vec1), 8);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  constexpr gf::Vec3I cvec1(5, -2, 1);
  static_assert(gf::chebyshev_distance(cvec0, cvec1) == 8, "Check Vec3");
}

TEST(Vec3Test, NaturalLength) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::natural_length(vec0), 62);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::natural_length(cvec0) == 62, "Check Vec3");
}

TEST(Vec3Test, NaturalDistance) {
  gf::Vec3I vec0(-3, 4, -5);
  gf::Vec3I vec1(5, -2, 1);

  EXPECT_EQ(gf::natural_distance(vec0, vec1), 156);

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  constexpr gf::Vec3I cvec1(5, -2, 1);
  static_assert(gf::natural_distance(cvec0, cvec1) == 156, "Check Vec3");
}

TEST(Vec3Test, Normalize) {
  gf::Vec3F vec0(4.0f, 4.0f, 4.0f);

  EXPECT_EQ(gf::normalize(vec0), gf::vec(gf::InvSqrt3, gf::InvSqrt3, gf::InvSqrt3));
}

TEST(Vec3Test, ProjX) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::projx(vec0), gf::vec(-3, 0, 0));

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::projx(cvec0) == gf::vec(-3, 0, 0), "Check Vec3");
}

TEST(Vec3Test, ProjY) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::projy(vec0), gf::vec(0, 4, 0));

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::projy(cvec0) == gf::vec(0, 4, 0), "Check Vec3");
}

TEST(Vec3Test, ProjZ) {
  gf::Vec3I vec0(-3, 4, -5);

  EXPECT_EQ(gf::projz(vec0), gf::vec(0, 0, -5));

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  static_assert(gf::projz(cvec0) == gf::vec(0, 0, -5), "Check Vec3");
}

TEST(Vec3Test, Cross) {
  gf::Vec3I vec0(-3, 4, -5);
  gf::Vec3I vec1(5, -2, 1);

  EXPECT_EQ(gf::cross(vec0, vec1), gf::vec(-6, -22, -14));

  constexpr gf::Vec3I cvec0(-3, 4, -5);
  constexpr gf::Vec3I cvec1(5, -2, 1);
  static_assert(gf::cross(cvec0, cvec1) == gf::vec(-6, -22, -14), "Check Vec3");
}
