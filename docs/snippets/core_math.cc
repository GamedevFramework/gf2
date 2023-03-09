#include <gf2/Math.h>

void doc_Step() {
// tag::step[]
  gf::Step<float> step0 = gf::cubic_step;
  gf::Step<double> step1 = gf::cosine_step;
  gf::Step<double> step2 = [](double t) { return t * t; };
// end::step[]
}
