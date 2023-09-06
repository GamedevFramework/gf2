#include <gf2/core/Move.h>

#include "gtest/gtest.h"

TEST(MoveTest, AngularFactor) {
  EXPECT_EQ(gf::angular_factor(gf::AngularMove::None), 0.0f);
  EXPECT_EQ(gf::angular_factor(gf::AngularMove::Left), -1.0f);
  EXPECT_EQ(gf::angular_factor(gf::AngularMove::Right), 1.0f);
}

TEST(MoveTest, LinearFactor) {
  EXPECT_EQ(gf::linear_factor(gf::LinearMove::None), 0.0f);
  EXPECT_EQ(gf::linear_factor(gf::LinearMove::Backward), -1.0f);
  EXPECT_EQ(gf::linear_factor(gf::LinearMove::Forward), 1.0f);
}
