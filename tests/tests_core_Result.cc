#include <type_traits>

#include <gf2/Result.h>

#include "gtest/gtest.h"

namespace {

  struct Data {
    Data() = default;

    Data(int i, double d)
    : i(i)
    , d(d)
    {
    }

    int i = 42;
    double d = 3.14;
  };

  enum class Error {
    Minor,
    Serious,
    Fatal,
  };

}

TEST(ResultTest, DefaultConstructor) {
  gf::Result<Data, Error> result;

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->i, 42);
  EXPECT_EQ(result->d, 3.14);
}

TEST(ResultTest, ValueConstructor) {
  gf::Result<Data, Error> result(Data(69, 1.44));

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->i, 69);
  EXPECT_EQ(result->d, 1.44);
}

TEST(ResultTest, EmplaceConstructor) {
  gf::Result<Data, Error> result(std::in_place, 69, 1.44);

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->i, 69);
  EXPECT_EQ(result->d, 1.44);
}

TEST(ResultTest, ErrorConstructor1) {
  gf::Result<Data, Error> result(gf::error(Error::Serious));

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), Error::Serious);
}

TEST(ResultTest, ErrorConstructor2) {
  auto error = gf::error(Error::Serious);
  gf::Result<Data, Error> result(error);

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), Error::Serious);
}

TEST(ResultTest, ValueAssignment1) {
  gf::Result<Data, Error> result;

  result = Data(69, 1.44);

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->i, 69);
  EXPECT_EQ(result->d, 1.44);
}

TEST(ResultTest, ValueAssignment2) {
  gf::Result<Data, Error> result;

  Data data(69, 1.44);
  result = data;

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->i, 69);
  EXPECT_EQ(result->d, 1.44);
}

TEST(ResultTest, ErrorAssignment1) {
  gf::Result<Data, Error> result;

  result = gf::error(Error::Serious);

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), Error::Serious);
}

TEST(ResultTest, ErrorAssignment2) {
  gf::Result<Data, Error> result;

  auto error = gf::error(Error::Serious);
  result = error;

  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), Error::Serious);
}

TEST(ResultTest, ValueOrWithValue) {
  gf::Result<Data, Error> result;

  auto data = result.value_or(Data(69, 1.44));
  EXPECT_EQ(data.i, 42);
  EXPECT_EQ(data.d, 3.14);
}

TEST(ResultTest, ValueOrWithError) {
  gf::Result<Data, Error> result = gf::error(Error::Serious);

  auto data = result.value_or(Data(69, 1.44));
  EXPECT_EQ(data.i, 69);
  EXPECT_EQ(data.d, 1.44);
}
