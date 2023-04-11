#include <gf2/Ref.h>

#include <type_traits>

#include "gtest/gtest.h"

namespace {

  struct Base {
  };

  struct Derived : Base {
  };

}

TEST(RefTest, Constructor) {
  int data = 42;
  gf::Ref<int> ref(data);

  EXPECT_EQ(ref.get(), 42);
  EXPECT_EQ(&ref.get(), &data);
}

TEST(RefTest, BaseConstructor) {
  Derived data;
  gf::Ref<Base> ref(data);

  EXPECT_EQ(&ref.get(), &data);
}

TEST(RefTest, ConvertingConstructor) {
  Derived data;
  gf::Ref<Derived> ref0(data);
  gf::Ref<Base> ref1(ref0);

  EXPECT_EQ(&ref0.get(), &ref1.get());
}

TEST(RefTest, Ref) {
  int data = 42;
  auto ref = gf::ref(data);

  static_assert(std::is_same_v<decltype(ref), gf::Ref<int>>, "Check Ref");
  EXPECT_EQ(ref.get(), 42);
  EXPECT_EQ(&ref.get(), &data);
}
