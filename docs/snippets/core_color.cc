#include <gf2/Color.h>

void doc_darker() {
// tag::darker[]
  gf::Color dark_red = gf::darker(gf::Red);
// end::darker[]
}

void doc_lighter() {
// tag::lighter[]
  gf::Color light_red = gf::lighter(gf::Red);
// end::lighter[]
}

void doc_opaque() {
// tag::opaque[]
  gf::Color semi_transparent_red = gf::Red * gf::opaque(0.5f);
// end::opaque[]
}
