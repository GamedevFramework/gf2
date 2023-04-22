#include <gf2/Clock.h>

void doc_Clock() {
  // tag::clock[]
  gf::Clock clock;
  // end::clock[]
  [[maybe_unused]]
  // tag::clock[]
  gf::Time time1 = clock.elapsed_time();
  // end::clock[]
  [[maybe_unused]]
  // tag::clock[]
  gf::Time time2 = clock.restart();
  // end::clock[]
}
