// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TOUCH_H
#define GF_TOUCH_H

#include <cstdint>

namespace gf {

  enum class TouchId : int64_t;

  constexpr TouchId MouseTouchId = TouchId{ -1 };

} // namespace gf

#endif // GF_TOUCH_H
