// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Ref.h>
// clang-format on

#include <type_traits>

namespace gf {

  static_assert(std::is_copy_constructible_v<Ref<int>>, "Ref should be copy constructible.");
  static_assert(std::is_copy_assignable_v<Ref<int>>, "Ref should be copy assignable.");

}
