// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Move.h>

#include <cassert>

namespace gf {

  float angular_factor(AngularMove move)
  {
    switch (move) {
      case AngularMove::None:
        return 0.0f;
      case AngularMove::Left:
        return -1.0f;
      case AngularMove::Right:
        return 1.0f;
    }

    assert(false);
    return 0.0f;
  }

  float linear_factor(LinearMove move)
  {
    switch (move) {
      case LinearMove::None:
        return 0.0f;
      case LinearMove::Backward:
        return -1.0f;
      case LinearMove::Forward:
        return 1.0f;
    }

    assert(false);
    return 0.0f;
  }

} // namespace gf
