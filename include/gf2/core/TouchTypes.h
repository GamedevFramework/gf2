// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TOUCH_TYPES_H
#define GF_TOUCH_TYPES_H

#include <cstdint>

#include <limits>
#include <type_traits>

namespace gf {

  enum class TouchId : uint64_t;

  constexpr TouchId MouseTouchId = TouchId{ std::numeric_limits<std::underlying_type_t<TouchId>>::max() };

  enum class FingerId : uint64_t;

} // namespace gf

#endif // GF_TOUCH_TYPES_H
