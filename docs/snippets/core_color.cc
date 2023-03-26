#include <gf2/Color.h>

void doc_darker() {
  [[maybe_unused]]
// tag::darker[]
  gf::Color dark_red = gf::darker(gf::Red);
// end::darker[]
}

void doc_lighter() {
  [[maybe_unused]]
// tag::lighter[]
  gf::Color light_red = gf::lighter(gf::Red);
// end::lighter[]
}

void doc_opaque() {
  [[maybe_unused]]
// tag::opaque[]
  gf::Color semi_transparent_red = gf::Red * gf::opaque(0.5f);
// end::opaque[]
}
