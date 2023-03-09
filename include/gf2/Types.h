// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TYPES_H
#define GF_TYPES_H

#include "CoreApi.h"

namespace gf {

  /**
   * @ingroup core_vocabulary
   * @brief Semantic type to represent "none"
   *
   * @sa gf::None, gf::AllType
   */
  struct GF_CORE_API NoneType { };

  /**
   * @ingroup core_vocabulary
   * @brief Constant to represent "none"
   *
   * @sa gf::NoneType, gf::All
   */
  constexpr NoneType None = { };

  /**
   * @ingroup core_vocabulary
   * @brief Semantic type to represent "all"
   *
   * @sa gf::All, gf::NoneType
   */
  struct GF_CORE_API AllType {  };

  /**
   * @ingroup core_vocabulary
   * @brief Constant to represent "all"
   *
   * @sa gf::AllType, gf::None
   */
  constexpr AllType All = { };

  /**
   * @ingroup core_vocabulary
   * @brief Semantic type to represent "any"
   *
   * @sa gf::Any, gf::AllType, gf::NoneType
   */
  struct GF_CORE_API AnyType {  };

  /**
   * @ingroup core_vocabulary
   * @brief Constant to represent "any"
   *
   * @sa gf::AnyType, gf::All, gf::None
   */
  constexpr AnyType Any = { };

  /**
   * @ingroup core_vocabulary
   * @brief Semantic type to represent "zero"
   *
   * @sa gf::Zero
   */
  struct GF_CORE_API ZeroType { };

  /**
   * @ingroup core_vocabulary
   * @brief Constant to represent "zero"
   *
   * @sa gf::ZeroType
   */
  constexpr ZeroType Zero = { };

}

#endif // GF_TYPES_H
