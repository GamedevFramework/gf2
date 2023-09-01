#include <gf2/Property.h>
#include <gf2/PropertyMap.h>

#include "gtest/gtest.h"

/*
 * Property
 */

TEST(PropertyTest, DefaultConstructor) {
  gf::Property property;

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, BoolConstructor) {
  gf::Property property(true);

  EXPECT_TRUE(property.is_bool());
  EXPECT_EQ(property.as_bool(), true);
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, IntConstructor) {
  gf::Property property(3);

  EXPECT_FALSE(property.is_bool());
  EXPECT_TRUE(property.is_int());
  EXPECT_EQ(property.as_int(), 3);
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, FloatConstructor) {
  gf::Property property(3.14);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_TRUE(property.is_float());
  EXPECT_EQ(property.as_float(), 3.14);
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, StringConstructor) {
  using namespace std::literals;
  gf::Property property("your base are belong to us."s);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_TRUE(property.is_string());
  EXPECT_EQ(property.as_string(), "your base are belong to us.");
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, PathConstructor) {
  const std::filesystem::path path("path/to/file");
  gf::Property property(path);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_TRUE(property.is_path());
  EXPECT_EQ(property.as_path(), path);
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, ObjectConstructor) {
  const auto object = gf::id(69);
  gf::Property property(object);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_TRUE(property.is_object());
  EXPECT_EQ(property.as_object(), object);
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, ColorConstructor) {
  gf::Property property(gf::Azure);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_TRUE(property.is_color());
  EXPECT_EQ(property.as_color(), gf::Azure);
  EXPECT_FALSE(property.is_class());
}

TEST(PropertyTest, ClassConstructor) {
  gf::PropertyMap property_map;
  property_map.add_property("key", 42);

  gf::Property property(property_map);

  EXPECT_FALSE(property.is_bool());
  EXPECT_FALSE(property.is_int());
  EXPECT_FALSE(property.is_float());
  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_path());
  EXPECT_FALSE(property.is_object());
  EXPECT_FALSE(property.is_color());
  EXPECT_TRUE(property.is_class());
  EXPECT_EQ(property.as_class(), property_map);
}

/*
 * PropertyMap
 */

TEST(PropertyMapTest, DefaultConstructor) {
  gf::PropertyMap property_map;

}

TEST(PropertyMapTest, Key) {
  gf::PropertyMap property_map;

  property_map.add_property("number", 42);
  property_map.add_property("color", gf::Blue);

  EXPECT_NO_THROW({
    auto number = property_map("number");
    EXPECT_TRUE(number.is_int());
    EXPECT_EQ(number.as_int(), 42);

    auto color = property_map("color");
    EXPECT_TRUE(color.is_color());
    EXPECT_EQ(color.as_color(), gf::Blue);
  });

  EXPECT_ANY_THROW({
    property_map("unknown");
  });
}

TEST(PropertyMapTest, Path) {
  gf::PropertyMap foo_property_map;
  foo_property_map.add_property("number", 42);

  gf::PropertyMap property_map;
  property_map.add_property("foo", std::move(foo_property_map));

  EXPECT_NO_THROW({
    auto number = property_map("foo/number");
    EXPECT_TRUE(number.is_int());
    EXPECT_EQ(number.as_int(), 42);

    auto foo = property_map("foo");
    EXPECT_TRUE(foo.is_class());
  });

  EXPECT_ANY_THROW({
    property_map("number");
  });
}
