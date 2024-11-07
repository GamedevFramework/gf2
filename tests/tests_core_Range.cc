#include <gf2/core/Range.h>

#include "gtest/gtest.h"

using RangeI = gf::Range<int>;
using RangeF = gf::Range<float>;

TEST(RangeTest, Constructor) {
  RangeI ri{ 0, 10 };

  EXPECT_EQ(0, ri.lo());
  EXPECT_EQ(10, ri.hi());
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

/*
 * NeighborSquareRange
 */

TEST(RangeTest, NeighborSquareCanonical) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborSquareRange<int> range = gf::neighbor_square_range({ 0, 0 }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((2 * radius + 1) * (2 * radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborSquareCorner) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborSquareRange<int> range(gf::range(0, radius + 1), gf::range(0, radius + 1), { 0, 0 });
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborSquareOppositeCorner) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborSquareRange<int> range(gf::range(0, radius + 1), gf::range(0, radius + 1), { radius, radius });
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborSquareHorizontalSide) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborSquareRange<int> range(gf::range(0, (2 * radius) + 1), gf::range(0, radius), { radius, 0 });
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, (radius * (2 * radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborSquareVerticalSide) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborSquareRange<int> range(gf::range(0, radius), gf::range(0, (2 * radius) + 1), { 0, radius });
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, (radius * (2 * radius + 1)) - 1);
  }
}

/*
 * NeighborDiamondRange
 */

TEST(RangeTest, NeighborDiamondCanonical) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborDiamondRange<int> range = gf::neighbor_diamond_range({ 0, 0 }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, (radius * radius) + ((radius + 1) * (radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborDiamondCorner) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborDiamondRange<int> range(gf::range(0, radius + 1), gf::range(0, radius + 1), { 0, 0 }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 2) / 2) - 1);
  }
}

TEST(RangeTest, NeighborDiamondOppositeCorner) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborDiamondRange<int> range(gf::range(0, radius + 1), gf::range(0, radius + 1), { radius, radius }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 2) / 2) - 1);
  }
}

TEST(RangeTest, NeighborDiamondHorizontalSide) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborDiamondRange<int> range(gf::range(0, (2 * radius) + 2), gf::range(0, radius + 1), { radius, 0 }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 1)) - 1);
  }
}

TEST(RangeTest, NeighborDiamondVerticalSide) {
  for (int radius = 1; radius < 5; ++radius) {
    const gf::NeighborDiamondRange<int> range(gf::range(0, radius + 1), gf::range(0, (2 * radius) + 2), { 0, radius }, radius);
    int count = 0;

    for ([[maybe_unused]] auto position : range) {
      ++count;
    }

    ASSERT_EQ(count, ((radius + 1) * (radius + 1)) - 1);
  }
}

/*
 * Enumerate
 */

TEST(RangeTest, Enumerate) {
  std::vector<double> vec = { 3.14, 2.2, 1.1 };

  for (auto [i, value ] : gf::enumerate(vec)) {
    EXPECT_EQ(vec[i], value);
  }

  for (auto&& [i, value ] : gf::enumerate(vec)) {
    static_assert(std::is_lvalue_reference_v<decltype(value)>);
    EXPECT_EQ(vec[i], value);
  }
}
