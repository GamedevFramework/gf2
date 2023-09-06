#include <cmath>
#include <cerrno>

#include <gf2/core/Result.h>

// tag::safe_sqrt[]
gf::Result<float, int> safe_sqrt(float x) {
  if (x < 0.0f) {
    return gf::error(EINVAL);
  }

  return std::sqrt(x);
}
// end::safe_sqrt[]
