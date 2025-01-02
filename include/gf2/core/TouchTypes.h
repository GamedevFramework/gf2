// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TOUCH_TYPES_H
#define GF_TOUCH_TYPES_H

#include <cstdint>

namespace gf {

  enum class TouchId : int64_t;

  constexpr TouchId MouseTouchId = TouchId{ -1 };

  enum class FingerId : int64_t;

} // namespace gf

#endif // GF_TOUCH_TYPES_H
