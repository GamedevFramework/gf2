#include <gf2/Rect.h>

#include "gtest/gtest.h"

TEST(RectTest, FromPositionSize) {
  gf::RectI ri = gf::RectI::from_position_size({ 5, 10 }, { 15, 20 });

  EXPECT_EQ(ri.offset.x, 5);
  EXPECT_EQ(ri.offset.y, 10);
  EXPECT_EQ(ri.extent.w, 15);
  EXPECT_EQ(ri.extent.h, 20);

  constexpr gf::RectI cri = gf::RectI::from_position_size({ 5, 10 }, { 15, 20 });
  static_assert(cri.offset.x == 5, "Check Rect");
  static_assert(cri.offset.y == 10, "Check Rect");
  static_assert(cri.extent.x == 15, "Check Rect");
  static_assert(cri.extent.y == 20, "Check Rect");
}

TEST(RectTest, FromSize) {
  gf::RectI ri = gf::RectI::from_size({ 15, 20 });

  EXPECT_EQ(ri.offset.x, 0);
  EXPECT_EQ(ri.offset.y, 0);
  EXPECT_EQ(ri.extent.w, 15);
  EXPECT_EQ(ri.extent.h, 20);

  constexpr gf::RectI cri = gf::RectI::from_size({ 15, 20 });
  static_assert(cri.offset.x == 0, "Check Rect");
  static_assert(cri.offset.y == 0, "Check Rect");
  static_assert(cri.extent.x == 15, "Check Rect");
  static_assert(cri.extent.y == 20, "Check Rect");
}

TEST(RectTest, FromMinMax) {
  gf::RectI ri = gf::RectI::from_min_max({ 5, 10 }, { 20, 30 });

  EXPECT_EQ(ri.offset.x, 5);
  EXPECT_EQ(ri.offset.y, 10);
  EXPECT_EQ(ri.extent.w, 15);
  EXPECT_EQ(ri.extent.h, 20);

  constexpr gf::RectI cri = gf::RectI::from_min_max({ 5, 10 }, { 20, 30 });
  static_assert(cri.offset.x == 5, "Check Rect");
  static_assert(cri.offset.y == 10, "Check Rect");
  static_assert(cri.extent.x == 15, "Check Rect");
  static_assert(cri.extent.y == 20, "Check Rect");
}

TEST(RectTest, FromCenterSize) {
  gf::RectI ri = gf::RectI::from_center_size({ 12, 20 }, { 15, 20 });

  EXPECT_EQ(ri.offset.x, 5);
  EXPECT_EQ(ri.offset.y, 10);
  EXPECT_EQ(ri.extent.w, 15);
  EXPECT_EQ(ri.extent.h, 20);

  constexpr gf::RectI cri = gf::RectI::from_center_size({ 12, 20 }, { 15, 20 });
  static_assert(cri.offset.x == 5, "Check Rect");
  static_assert(cri.offset.y == 10, "Check Rect");
  static_assert(cri.extent.x == 15, "Check Rect");
  static_assert(cri.extent.y == 20, "Check Rect");
}

TEST(RectTest, Empty) {
  gf::RectI ri1 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });
  EXPECT_FALSE(ri1.empty());

  gf::RectI ri2 = gf::RectI::from_position_size({ 0, 5 }, { 0, 15 });
  EXPECT_TRUE(ri2.empty());

  gf::RectI ri3 = gf::RectI::from_position_size({ 0, 5 }, { 10, 0 });
  EXPECT_TRUE(ri3.empty());

  gf::RectI ri4 = gf::RectI::from_position_size({ 0, 5 }, { 0, 0 });
  EXPECT_TRUE(ri4.empty());

  constexpr gf::RectI cri1 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });
  static_assert(!cri1.empty(), "Check Rect");

  constexpr gf::RectI cri2 = gf::RectI::from_position_size({ 0, 5 }, { 0, 15 });
  static_assert(cri2.empty(), "Check Rect");

  constexpr gf::RectI cri3 = gf::RectI::from_position_size({ 0, 5 }, { 10, 0 });
  static_assert(cri3.empty(), "Check Rect");

  constexpr gf::RectI cri4 = gf::RectI::from_position_size({ 0, 5 }, { 0, 0 });
  static_assert(cri4.empty(), "Check Rect");
}

TEST(RectTest, ContainsVec) {
  gf::RectI ri = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  EXPECT_TRUE( ri.contains({  5, 10 })); // inside point
  EXPECT_FALSE(ri.contains({  0,  0 })); // outside point
  EXPECT_TRUE( ri.contains({  0,  5 })); // top-left
  EXPECT_FALSE(ri.contains({ 10,  5 })); // top-right
  EXPECT_FALSE(ri.contains({  0, 20 })); // bottom-left
  EXPECT_FALSE(ri.contains({ 10, 20 })); // bottom-right

  constexpr gf::RectI cri = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });
  static_assert( cri.contains({  5, 10 }), "Chech Rect"); // inside point
  static_assert(!cri.contains({  0,  0 }), "Chech Rect"); // outside point
  static_assert( cri.contains({  0,  5 }), "Chech Rect"); // top-left
  static_assert(!cri.contains({ 10,  5 }), "Chech Rect"); // top-right
  static_assert(!cri.contains({  0, 20 }), "Chech Rect"); // bottom-left
  static_assert(!cri.contains({ 10, 20 }), "Chech Rect"); // bottom-right

}

TEST(RectTest, ContainsRect) {
  gf::RectI ri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  EXPECT_TRUE(ri0.contains(ri0));

  // inside
  gf::RectI ri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  EXPECT_TRUE(ri0.contains(ri1));
  EXPECT_FALSE(ri1.contains(ri0));

  // outside
  gf::RectI ri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  EXPECT_FALSE(ri0.contains(ri2));
  EXPECT_FALSE(ri2.contains(ri0));

  // crossing
  gf::RectI ri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  EXPECT_FALSE(ri0.contains(ri3));
  EXPECT_FALSE(ri3.contains(ri0));

  // corner
  gf::RectI ri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  EXPECT_FALSE(ri0.contains(ri4));
  EXPECT_FALSE(ri4.contains(ri0));

  constexpr gf::RectI cri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  static_assert(cri0.contains(cri0), "Check Rect");

  // inside
  constexpr gf::RectI cri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  static_assert(cri0.contains(cri1), "Check Rect");
  static_assert(!cri1.contains(cri0), "Check Rect");

  // outside
  constexpr gf::RectI cri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  static_assert(!cri0.contains(cri2), "Check Rect");
  static_assert(!cri2.contains(cri0), "Check Rect");

  // crossing
  constexpr gf::RectI cri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  static_assert(!cri0.contains(cri3), "Check Rect");
  static_assert(!cri3.contains(cri0), "Check Rect");

  // corner
  constexpr gf::RectI cri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  static_assert(!cri0.contains(cri4), "Check Rect");
  static_assert(!cri4.contains(cri0), "Check Rect");
}

TEST(RectTest, Intersects) {
  gf::RectI ri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  EXPECT_TRUE(ri0.intersects(ri0));

  // inside
  gf::RectI ri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  EXPECT_TRUE(ri0.intersects(ri1));
  EXPECT_TRUE(ri1.intersects(ri0));

  // outside
  gf::RectI ri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  EXPECT_FALSE(ri0.intersects(ri2));
  EXPECT_FALSE(ri2.intersects(ri0));

  // crossing
  gf::RectI ri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  EXPECT_TRUE(ri0.intersects(ri3));
  EXPECT_TRUE(ri3.intersects(ri0));

  // corner
  gf::RectI ri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  EXPECT_FALSE(ri0.intersects(ri4));
  EXPECT_FALSE(ri4.intersects(ri0));

  constexpr gf::RectI cri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  static_assert(cri0.intersects(cri0), "Check Rect");

  // inside
  constexpr gf::RectI cri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  static_assert(cri0.intersects(cri1), "Check Rect");
  static_assert(cri1.intersects(cri0), "Check Rect");

  // outside
  constexpr gf::RectI cri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  static_assert(!cri0.intersects(cri2), "Check Rect");
  static_assert(!cri2.intersects(cri0), "Check Rect");

  // crossing
  constexpr gf::RectI cri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  static_assert(cri0.intersects(cri3), "Check Rect");
  static_assert(cri3.intersects(cri0), "Check Rect");

  // corner
  constexpr gf::RectI cri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  static_assert(!cri0.intersects(cri4), "Check Rect");
  static_assert(!cri4.intersects(cri0), "Check Rect");

}

TEST(RectTest, Intersection) {
  gf::RectI ri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  auto intersection0 = ri0.intersection(ri0);
  EXPECT_TRUE(intersection0);
  EXPECT_EQ(*intersection0, ri0);

  // inside
  gf::RectI ri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  auto intersection1 = ri0.intersection(ri1);
  EXPECT_TRUE(intersection1);
  EXPECT_EQ(*intersection1, ri1);

  // outside
  gf::RectI ri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  auto intersection2 = ri0.intersection(ri2);
  EXPECT_FALSE(intersection2);

  // crossing
  gf::RectI ri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  auto intersection3 = ri0.intersection(ri3);
  EXPECT_TRUE(intersection3);
  EXPECT_EQ(*intersection3, gf::RectI::from_position_size({ 2, 5 }, { 8, 13 }));

  // corner
  gf::RectI ri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  auto intersection4 = ri0.intersection(ri4);
  EXPECT_FALSE(intersection4);

  constexpr gf::RectI cri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // reflexivity
  constexpr auto cintersection0 = cri0.intersection(cri0);
  static_assert(cintersection0, "Check Rect");
  static_assert(*cintersection0 == cri0, "Check Rect");

  // inside
  constexpr gf::RectI cri1 = gf::RectI::from_position_size({ 1, 6 }, { 1, 1 });
  constexpr auto cintersection1 = cri0.intersection(cri1);
  static_assert(cintersection1, "Check Rect");
  static_assert(*cintersection1 == cri1, "Check Rect");

  // outside
  constexpr gf::RectI cri2 = gf::RectI::from_position_size({ 0, 0 }, { 1, 1 });
  constexpr auto cintersection2 = cri0.intersection(cri2);
  static_assert(!cintersection2, "Check Rect");

  // crossing
  constexpr gf::RectI cri3 = gf::RectI::from_position_size({ 2, 3 }, { 10, 15 });
  constexpr auto cintersection3 = cri0.intersection(cri3);
  static_assert(cintersection3, "Check Rect");
  static_assert(*cintersection3 == gf::RectI::from_position_size({ 2, 5 }, { 8, 13 }), "Check Rect");

  // corner
  constexpr gf::RectI cri4 = gf::RectI::from_position_size({ 10, 20 }, { 10, 15 });
  constexpr auto cintersection4 = cri0.intersection(cri4);
  static_assert(!cintersection4);
}

TEST(RectTest, ExtendToVec) {
  gf::RectI ri0 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 });

  // outside (top left)
  ri0.extend_to({ -5, 0 });
  EXPECT_EQ(ri0, gf::RectI::from_position_size({ -5, 0 }, { 15, 20 }));

  // outside (bottom right)
  ri0.extend_to({ 25, 30 });
  EXPECT_EQ(ri0, gf::RectI::from_position_size({ -5, 0 }, { 30, 30 }));

  // inside
  ri0.extend_to({ 10, 10 });
  EXPECT_EQ(ri0, gf::RectI::from_position_size({ -5, 0 }, { 30, 30 }));

  // outside (top left)
  constexpr gf::RectI cri1 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 }).extend_to({ -5, 0 });
  static_assert(cri1 == gf::RectI::from_position_size({ -5, 0 }, { 15, 20 }), "Check Rect");

  // outside (bottom right)
  constexpr gf::RectI cri2 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 }).extend_to({ 15, 25 });
  static_assert(cri2 == gf::RectI::from_position_size({ 0, 5 }, { 15, 20 }), "Check Rect");

  // inside
  constexpr gf::RectI cri3 = gf::RectI::from_position_size({ 0, 5 }, { 10, 15 }).extend_to({ 10, 10 });
  static_assert(cri3 == gf::RectI::from_position_size({ 0, 5 }, { 10, 15 }), "Check Rect");
}
