#include <gf2/core/Dice.h>

#include "gtest/gtest.h"

using namespace gf::literals;

TEST(DiceTest, Ctor) {
  gf::Dice dice(3, 2, 4);

  EXPECT_EQ(dice.faces(), 3);
  EXPECT_EQ(dice.count(), 2);
  EXPECT_EQ(dice.modifier(), 4);
}

TEST(DiceTest, Faces) {
  gf::Dice dice = "d3"_dice;

  EXPECT_EQ(dice.faces(), 3);
  EXPECT_EQ(dice.count(), 1);
  EXPECT_EQ(dice.modifier(), 0);
}

TEST(DiceTest, FacesAndCount) {
  gf::Dice dice = "2D3"_dice;

  EXPECT_EQ(dice.faces(), 3);
  EXPECT_EQ(dice.count(), 2);
  EXPECT_EQ(dice.modifier(), 0);
}

TEST(DiceTest, FacesCountAndModifier) {
  gf::Dice dice = "2d3+4"_dice;

  EXPECT_EQ(dice.faces(), 3);
  EXPECT_EQ(dice.count(), 2);
  EXPECT_EQ(dice.modifier(), 4);
}

TEST(DiceTest, FacesAndModifier) {
  gf::Dice dice = "D3+4"_dice;

  EXPECT_EQ(dice.faces(), 3);
  EXPECT_EQ(dice.count(), 1);
  EXPECT_EQ(dice.modifier(), 4);
}

TEST(DiceTest, Constexpr) {
  constexpr gf::Dice Dice = "2d3+4"_dice;

  static_assert(Dice.faces() == 3, "Dice::faces()");
  static_assert(Dice.count() == 2, "Dice::count()");
  static_assert(Dice.modifier() == 4, "Dice::modifier()");

  EXPECT_EQ(Dice.faces(), 3);
  EXPECT_EQ(Dice.count(), 2);
  EXPECT_EQ(Dice.modifier(), 4);
}

TEST(DiceTest, Roll) {
  constexpr gf::Dice dice = "2D3+4"_dice;

  gf::Random random;

  for (int i = 0; i < 1000; ++i) {
    int x = dice.roll(&random);
    ASSERT_GE(x, 6);
    ASSERT_LE(x, 10);
  }
}
