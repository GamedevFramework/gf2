#include <cassert>

#include <string>

#include <gf2/core/Id.h>

// tag::using[]
using namespace gf::literals; // necessary to use _id
// end::using[]

namespace {

  std::string my_input() {
    return "";
  }

  template <typename... Args>
  constexpr void unused(Args&&...) { }

}

void doc_hash() {
  // tag::hash[]
  gf::Id id1 = gf::hash_string("C string");

  std::string str("std::string");
  gf::Id id2 = gf::hash_string(str);
  // end::hash[]

  unused(id1, id2);
}

void doc_udl()  {
  // tag::udl[]
  gf::Id id1 = "C string"_id;
  gf::Id id2 = "std::string"_id;
  // end::udl[]

  unused(id1, id2);
}

void doc_comptime()  {
  // tag::comptime[]
  static_assert("foobar"_id == gf::id(0x85944171f73967e8), "It works at compile-time!");
  // end::comptime[]
}

void doc_switch() {
  // tag::switch[]
  std::string input = my_input();

  switch (gf::hash_string(input)) {
    case "Foo"_id:
      // input is "Foo"
      break;
    case "Bar"_id:
      // input is "Bar"
      break;
    case "Baz"_id:
      // input is "Baz"
      break;
  }
  // end::switch[]
}
