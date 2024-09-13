#include <gf2/core/Fixed.h>

#include "gtest/gtest.h"

TEST(FixedTest, DefaultConstructor) {
  gf::Fixed32 fixed;

  EXPECT_EQ(fixed.raw(), 0);
}

TEST(FixedTest, Addition) {
  gf::Fixed32 a = 2;

  gf::Fixed32 b = a + 2;
  EXPECT_EQ(b.as_int(), 4);

  gf::Fixed32 c = 2 + b;
  EXPECT_EQ(c.as_int(), 6);

  gf::Fixed32 d = b + c;
  EXPECT_EQ(d.as_int(), 10);
}

TEST(FixedTest, Substraction) {
  gf::Fixed32 a = 3;

  gf::Fixed32 b = a - 2;
  EXPECT_EQ(b.as_int(), 1);

  gf::Fixed32 c = 2 - b;
  EXPECT_EQ(c.as_int(), 1);

  gf::Fixed32 d = b - c;
  EXPECT_EQ(d.as_int(), 0);
}

TEST(FixedTest, Multiplication) {
  gf::Fixed32 a = 2;

  gf::Fixed32 b = a * 2;
  EXPECT_EQ(b.as_int(), 4);

  gf::Fixed32 c = 2 * b;
  EXPECT_EQ(c.as_int(), 8);

  gf::Fixed32 d = b * c;
  EXPECT_EQ(d.as_int(), 32);
}

TEST(FixedTest, Division) {
  gf::Fixed32 a = 8;

  gf::Fixed32 b = a / 2;
  EXPECT_EQ(b.as_int(), 4);

  gf::Fixed32 c = 20 / b;
  EXPECT_EQ(c.as_int(), 5);

  gf::Fixed32 d = b / c;
  EXPECT_EQ(d.as_int(), 0);
}
