#include <gf2/core/Id.h>

#include "gtest/gtest.h"

struct FNVTestCase {
  const char* input;
  uint64_t expected;
};

TEST(IdTest, FNV) {
  // test vectors from:
  // http://www.isthe.com/chongo/src/fnv/test_fnv.c

  static constexpr FNVTestCase TestVectors[] = {
    {      "", UINT64_C(0xcbf29ce484222325)},
    {     "a", UINT64_C(0xaf63dc4c8601ec8c)},
    {     "b", UINT64_C(0xaf63df4c8601f1a5)},
    {     "c", UINT64_C(0xaf63de4c8601eff2)},
    {     "d", UINT64_C(0xaf63d94c8601e773)},
    {     "e", UINT64_C(0xaf63d84c8601e5c0)},
    {     "f", UINT64_C(0xaf63db4c8601ead9)},
    {    "fo", UINT64_C(0x08985907b541d342)},
    {   "foo", UINT64_C(0xdcb27518fed9d577)},
    {  "foob", UINT64_C(0xdd120e790c2512af)},
    { "fooba", UINT64_C(0xcac165afa2fef40a)},
    {"foobar", UINT64_C(0x85944171f73967e8)},
  };

  for (auto& test : TestVectors) {
    EXPECT_EQ(gf::hash(test.input), gf::id(test.expected));
  }
}

TEST(IdTest, Hash) {
  gf::Id id = gf::hash("foobar");
  EXPECT_EQ(id, gf::id(UINT64_C(0x85944171f73967e8)));

  constexpr gf::Id cid = gf::hash("foobar");
  static_assert(cid == gf::id(UINT64_C(0x85944171f73967e8)), "Check gf::Id");
}

TEST(IdTest, UDL) {
  using namespace gf::literals;

  gf::Id id = "foobar"_id;
  EXPECT_EQ(id, gf::id(UINT64_C(0x85944171f73967e8)));

  constexpr gf::Id cid = "foobar"_id;
  static_assert(cid == gf::id(UINT64_C(0x85944171f73967e8)), "Check gf::Id");
}
