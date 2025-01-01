#include <gf2/core/Rational.h>

#include "gtest/gtest.h"

TEST(RationalTest, DefaultConstructor) {
  const gf::Rational<int> r;

  EXPECT_EQ(r.numerator(), 0);
  EXPECT_EQ(r.denominator(), 1);
}

TEST(RationalTest, IntegerConstructor) {
  const gf::Rational<int> r(3);

  EXPECT_EQ(r.numerator(), 3);
  EXPECT_EQ(r.denominator(), 1);
}

TEST(RationalTest, FullConstructor) {
  const gf::Rational<int> r(5, 2);

  EXPECT_EQ(r.numerator(), 5);
  EXPECT_EQ(r.denominator(), 2);
}

TEST(RationalTest, FullNormalizedConstructor) {
  const gf::Rational<int> r(10, 4);

  EXPECT_EQ(r.numerator(), 5);
  EXPECT_EQ(r.denominator(), 2);
}

TEST(RationalTest, ExceptionConstructor) {
  EXPECT_THROW({ gf::Rational<int> r(1, 0); }, std::runtime_error); // NOLINT
}

TEST(RationalTest, Opposite) {
  const gf::Rational<int> r1(5, 2);

  auto r2 = +r1;

  EXPECT_EQ(r2.numerator(), 5);
  EXPECT_EQ(r2.denominator(), 2);

  auto r3 = -r1;

  EXPECT_EQ(r3.numerator(), -5);
  EXPECT_EQ(r3.denominator(), 2);
}

TEST(RationalTest, Addition) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(3, 7);

  auto r3 = r1 + r2;

  EXPECT_EQ(r3.numerator(), 41);
  EXPECT_EQ(r3.denominator(), 14);

  auto r4 = r1 + 2;

  EXPECT_EQ(r4.numerator(), 9);
  EXPECT_EQ(r4.denominator(), 2);

  auto r5 = 2 + r1;

  EXPECT_EQ(r5.numerator(), 9);
  EXPECT_EQ(r5.denominator(), 2);
}

TEST(RationalTest, Substraction) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(3, 7);

  auto r3 = r1 - r2;

  EXPECT_EQ(r3.numerator(), 29);
  EXPECT_EQ(r3.denominator(), 14);

  auto r4 = r1 - 2;

  EXPECT_EQ(r4.numerator(), 1);
  EXPECT_EQ(r4.denominator(), 2);

  auto r5 = 2 - r1;

  EXPECT_EQ(r5.numerator(), -1);
  EXPECT_EQ(r5.denominator(), 2);

  auto r6 = r2 - r2; // NOLINT(misc-redundant-expression)

  EXPECT_EQ(r6.numerator(), 0);
  EXPECT_EQ(r6.denominator(), 1);
}

TEST(RationalTest, Multiplication) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(3, 7);

  auto r3 = r1 * r2;

  EXPECT_EQ(r3.numerator(), 15);
  EXPECT_EQ(r3.denominator(), 14);

  auto r4 = r1 * 2;

  EXPECT_EQ(r4.numerator(), 5);
  EXPECT_EQ(r4.denominator(), 1);

  auto r5 = 2 * r1;

  EXPECT_EQ(r5.numerator(), 5);
  EXPECT_EQ(r5.denominator(), 1);
}

TEST(RationalTest, Division) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(3, 7);

  auto r3 = r1 / r2;

  EXPECT_EQ(r3.numerator(), 35);
  EXPECT_EQ(r3.denominator(), 6);

  auto r4 = r1 / 2;

  EXPECT_EQ(r4.numerator(), 5);
  EXPECT_EQ(r4.denominator(), 4);

  auto r5 = 2 / r1;

  EXPECT_EQ(r5.numerator(), 4);
  EXPECT_EQ(r5.denominator(), 5);

  auto r6 = r2 / r2; // NOLINT(misc-redundant-expression)

  EXPECT_EQ(r6.numerator(), 1);
  EXPECT_EQ(r6.denominator(), 1);
}

TEST(RationalTest, DivisionByZero) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(0, 7);

  EXPECT_THROW({ [[maybe_unused]] auto r3 = r1 / r2; }, std::runtime_error); // NOLINT
}

TEST(RationalTest, Comparison) {
  const gf::Rational<int> r1(5, 2);
  const gf::Rational<int> r2(3, 7);

  EXPECT_FALSE(r1 < r2);
  EXPECT_TRUE(r2 < r1);

  EXPECT_FALSE(r1 <= r2);
  EXPECT_TRUE(r2 <= r1);

  EXPECT_TRUE(r1 > r2);
  EXPECT_FALSE(r2 > r1);

  EXPECT_TRUE(r1 >= r2);
  EXPECT_FALSE(r2 >= r1);

  EXPECT_TRUE(r1 == r1);
  EXPECT_FALSE(r1 == r2);

  EXPECT_FALSE(r1 != r1);
  EXPECT_TRUE(r1 != r2);

  EXPECT_FALSE(r1 < 2);
  EXPECT_TRUE(2 < r1);

  EXPECT_FALSE(r1 <= 2);
  EXPECT_TRUE(2 <= r1);

  EXPECT_TRUE(r1 > 2);
  EXPECT_FALSE(2 > r1);

  EXPECT_TRUE(r1 >= 2);
  EXPECT_FALSE(2 >= r1);

  EXPECT_FALSE(r1 == 2);
  EXPECT_TRUE(r1 != 2);
}
