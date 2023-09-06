#include <gf2/core/Time.h>

void doc_Time() {
  // tag::time[]
  gf::Time t1 = gf::seconds(0.1f);
  // end::time[]
  [[maybe_unused]]
  // tag::time[]
  int32_t milli = t1.as_milliseconds(); // 100

  gf::Time t2 = gf::milliseconds(30);
  // end::time[]
  [[maybe_unused]]
  // tag::time[]
  int64_t micro = t2.as_microseconds(); // 30000

  gf::Time t3 = gf::microseconds(-800000);
  // end::time[]
  [[maybe_unused]]
  // tag::time[]
  float sec = t3.as_seconds(); // -0.8
  // end::time[]
}

void doc_seconds() {
  // tag::time_s[]
  using namespace gf::literals;
  // end::time_s[]
  [[maybe_unused]]
  // tag::time_s[]
  gf::Time time = 66.6_seconds;
  // end::time_s[]
}

void doc_milliseconds() {
  // tag::time_ms[]
  using namespace gf::literals;
  // end::time_ms[]
  [[maybe_unused]]
  // tag::time_ms[]
  gf::Time time = 42_milliseconds;
  // end::time_ms[]
}

void doc_microseconds() {
  // tag::time_us[]
  using namespace gf::literals;
  // end::time_us[]
  [[maybe_unused]]
  // tag::time_us[]
  gf::Time time = 13_microseconds;
  // end::time_us[]
}
