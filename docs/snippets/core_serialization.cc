#include <gf2/core/Serialization.h>

#include <string>

// tag::user_defined[]
struct Foo {
  double bar;
  int32_t baz;
  std::string qux;
};

template<typename Archive>
Archive& operator|(Archive& ar, Foo& foo) {
  return ar | foo.bar | foo.baz | foo.qux;
}
// end::user_defined[]

// tag::version[]
struct Entity {
  uint64_t id;
  uint64_t timestamp; // added in version 2
};

template<typename Archive>
Archive& operator|(Archive& ar, Entity& entity) {
  ar | entity.id;

  if (ar.version() >= 2) {
    ar | entity.timestamp;
  }

  return ar;
}
// end::version[]

void doc_Serialisation() {

}
