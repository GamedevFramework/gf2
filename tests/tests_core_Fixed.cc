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

TEST(FixedTest, Ceil) {
  gf::Fixed32 a = 8.2f;
  EXPECT_EQ(a.ceil(), 9);

  gf::Fixed32 b = 8.0f;
  EXPECT_EQ(b.ceil(), 8);

  gf::Fixed32 c = -8.2f;
  EXPECT_EQ(c.ceil(), -8);

  gf::Fixed32 d = -8.0f;
  EXPECT_EQ(d.ceil(), -8);
}

TEST(FixedTest, Floor) {
  gf::Fixed32 a = 8.2f;
  EXPECT_EQ(a.floor(), 8);

  gf::Fixed32 b = 8.0f;
  EXPECT_EQ(b.floor(), 8);

  gf::Fixed32 c = -8.2f;
  EXPECT_EQ(c.floor(), -9);

  gf::Fixed32 d = -8.0f;
  EXPECT_EQ(d.floor(), -8);
}

TEST(FixedTest, Equality) {
  gf::Fixed32 a = 3.4f;
  gf::Fixed32 b = 5;

  EXPECT_EQ(a, a);
  EXPECT_EQ(b, b);
  EXPECT_EQ(a, 3.4f);
  EXPECT_EQ(3.4f, a);
  EXPECT_EQ(5, b);
  EXPECT_EQ(b, 5);

  EXPECT_NE(a, b);
  EXPECT_NE(b, a);
  EXPECT_NE(a, 5);
  EXPECT_NE(5, a);
  EXPECT_NE(b, 3.4f);
  EXPECT_NE(3.4f, b);
}

TEST(FixedTest, Comparison) {
  gf::Fixed32 a = 3.4f;
  gf::Fixed32 b = 5;

  EXPECT_LE(a, b);
  EXPECT_LE(a, 5);
  EXPECT_LE(1, a);
  EXPECT_LE(a, 5.6f);
  EXPECT_LE(1.2f, a);
  EXPECT_LE(b, 6);
  EXPECT_LE(4, b);
  EXPECT_LE(b, 6.7f);
  EXPECT_LE(3.4f, b);

  EXPECT_LT(a, b);
  EXPECT_LT(a, 5);
  EXPECT_LT(1, a);
  EXPECT_LT(a, 5.6f);
  EXPECT_LT(1.2f, a);
  EXPECT_LT(b, 6);
  EXPECT_LT(4, b);
  EXPECT_LT(b, 6.7f);
  EXPECT_LT(3.4f, b);

  EXPECT_GE(b, a);
  EXPECT_GE(5, a);
  EXPECT_GE(a, 1);
  EXPECT_GE(5.6f, a);
  EXPECT_GE(a, 1.2f);
  EXPECT_GE(6, b);
  EXPECT_GE(b, 4);
  EXPECT_GE(6.7f, b);
  EXPECT_GE(b, 3.4f);

  EXPECT_GT(b, a);
  EXPECT_GT(5, a);
  EXPECT_GT(a, 1);
  EXPECT_GT(5.6f, a);
  EXPECT_GT(a, 1.2f);
  EXPECT_GT(6, b);
  EXPECT_GT(b, 4);
  EXPECT_GT(6.7f, b);
  EXPECT_GT(b, 3.4f);

}
