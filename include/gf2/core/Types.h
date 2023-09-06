// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TYPES_H
#define GF_TYPES_H

#include "CoreApi.h"

namespace gf {

  struct GF_CORE_API NoneType { };
  constexpr NoneType None = {};

  struct GF_CORE_API AllType { };
  constexpr AllType All = {};

  struct GF_CORE_API AnyType { };
  constexpr AnyType Any = {};

  struct GF_CORE_API ZeroType { };
  constexpr ZeroType Zero = {};

} // namespace gf

#endif // GF_TYPES_H
