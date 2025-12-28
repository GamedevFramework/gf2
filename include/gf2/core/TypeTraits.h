// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TYPE_TRAITS_H
#define GF_TYPE_TRAITS_H

#include <type_traits>

namespace gf {

  template<typename Data, typename Archive>
  using MaybeConst = Archive::template Constness<Data>;

  template<typename Parameter, typename Class>
  using PreventHiddenCopyMove = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Parameter>>, Class>>;

}

#endif // GF_TYPE_TRAITS_H
