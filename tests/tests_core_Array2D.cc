#include <gf2/core/Array2D.h>

#include "gtest/gtest.h"

constexpr gf::Vec2I Size = { 7, 7 };

TEST(Array2DTest, PositionRange) {
  gf::Array2D<char> array(Size);
  int count = 0;

  for (auto position : array.position_range()) {
    ASSERT_TRUE(array.valid(position));
    ++count;
  }

  // clang-format off
  EXPECT_EQ(count, Size.x * Size.y);
  // clang-format on
}

TEST(Array2DTest, FourNeighbors) {
  gf::Array2D<char> array(Size);

  for (auto position : array.position_range()) {
    for (auto neighbor : array.compute_4_neighbors_range(position)) {
      ASSERT_TRUE(array.valid(neighbor));
    }
  }
}

TEST(Array2DTest, EightNeighbors) {
  gf::Array2D<char> array(Size);

  for (auto position : array.position_range()) {
    for (auto neighbor : array.compute_8_neighbors_range(position)) {
      ASSERT_TRUE(array.valid(neighbor));
    }
  }
}

TEST(Array2DTest, TwelveNeighbors) {
  gf::Array2D<char> array(Size);

  for (auto position : array.position_range()) {
    for (auto neighbor : array.compute_12_neighbors_range(position)) {
      ASSERT_TRUE(array.valid(neighbor));
    }
  }
}

TEST(Array2DTest, TwentyFourNeighbors) {
  gf::Array2D<char> array(Size);

  for (auto position : array.position_range()) {
    for (auto neighbor : array.compute_24_neighbors_range(position)) {
      ASSERT_TRUE(array.valid(neighbor));
    }
  }
}
