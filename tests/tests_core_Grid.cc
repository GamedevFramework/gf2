#include <gf2/core/GridMap.h>

#include "gtest/gtest.h"

TEST(GridTest, DefaultConstructor) {
  gf::GridMap map;

  EXPECT_EQ(map.size(), gf::vec(0, 0));
}

TEST(GridTest, Dijkstra) {
  constexpr int Size = 10;
  gf::GridMap map = gf::GridMap::make_orthogonal({ Size, Size });

  gf::RouteCost cost;
  cost.cardinal = 1.0f;
  cost.diagonal = 0.0f;
  cost.blocked = 1.0f;

  auto path = map.compute_route({ 0, 0 }, { Size - 1, Size - 1}, cost, gf::Route::Dijkstra);

  EXPECT_EQ(path.size(), 2 * Size - 1);
}

TEST(GridTest, AStar) {
  constexpr int Size = 10;
  gf::GridMap map = gf::GridMap::make_orthogonal({ Size, Size });

  gf::RouteCost cost;
  cost.cardinal = 1.0f;
  cost.diagonal = 0.0f;
  cost.blocked = 1.0f;

  auto path = map.compute_route({ 0, 0 }, { Size - 1, Size - 1}, cost, gf::Route::AStar);

  EXPECT_EQ(path.size(), 2 * Size - 1);
}
