#include <gf2/Range.h>

#include "gtest/gtest.h"

using RangeI = gf::Range<int>;
using RangeF = gf::Range<float>;

TEST(RangeTest, Constructor) {
  RangeI ri{ 0, 10 };

  EXPECT_EQ(0, ri.lo);
  EXPECT_EQ(10, ri.hi);
}

TEST(RangeTest, Contains) {
  RangeI ri1{ 1, 10 };

  EXPECT_TRUE(ri1.contains(1));
  EXPECT_TRUE(!ri1.contains(10));

  EXPECT_TRUE(ri1.contains(5));
  EXPECT_TRUE(!ri1.contains(0));
  EXPECT_TRUE(!ri1.contains(11));

  RangeF ri2{ 1.0f, 10.0f };

  EXPECT_TRUE(ri2.contains(1.0f));
  EXPECT_TRUE(!ri2.contains(10.0f));

  EXPECT_TRUE(ri2.contains(5.0f));
  EXPECT_TRUE(!ri2.contains(0.0f));
  EXPECT_TRUE(!ri2.contains(11.0f));

  static_assert(RangeI{ 1, 10 }.contains(1), "Chech Range");
  static_assert(!RangeI{ 1, 10 }.contains(10), "Chech Range");

  static_assert(RangeI{ 1, 10 }.contains(5), "Chech Range");
  static_assert(!RangeI{ 1, 10 }.contains(0), "Chech Range");
  static_assert(!RangeI{ 1, 10 }.contains(11), "Chech Range");
}

TEST(RangeTest, Size) {
  RangeI ri1{ 1, 10 };
  EXPECT_EQ(9, ri1.size());

  RangeF ri2{ 1.0, 10.0 };
  ASSERT_FLOAT_EQ(9.0, ri2.size());

  static_assert(RangeI{ 1, 10 }.size() == 9, "Chech Range");
}

TEST(RangeTest, Empty) {
  RangeI ri1{ 1, 10 };
  EXPECT_TRUE(!ri1.empty());

  RangeI ri2{ 1, 1 };
  EXPECT_TRUE(ri2.empty());

  RangeI ri3{ 10, 1 };
  EXPECT_TRUE(ri3.empty());

  static_assert(!RangeI{ 1, 10 }.empty(), "Chech Range");
  static_assert(RangeI{ 1, 1 }.empty(), "Chech Range");
  static_assert(RangeI{ 10, 1 }.empty(), "²");
}

TEST(RangeTest, Valid) {
  RangeI ri1{ 1, 10 };
  EXPECT_TRUE(ri1.valid());

  RangeI ri2{ 1, 1 };
  EXPECT_TRUE(ri2.valid());

  RangeI ri3{ 10, 1 };
  EXPECT_TRUE(!ri3.valid());

  static_assert(RangeI{ 1, 10 }.valid(), "Chech Range");
  static_assert(RangeI{ 1, 1 }.valid(), "Chech Range");
  static_assert(!RangeI{ 10, 1 }.valid(), "Chech Range");
}

TEST(RangeTest, BeginEnd) {
  RangeI ri{ 1, 10 };

  int count = 0;

  for (auto x : ri) {
    ASSERT_TRUE(ri.contains(x));
    count++;
  }

  ASSERT_EQ(count, ri.size());
}
