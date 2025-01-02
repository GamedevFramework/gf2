// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RESOURCE_CONTEXT_H
#define GF_RESOURCE_CONTEXT_H

#include <type_traits>

namespace gf {

  namespace details {
    template<typename T, typename = std::void_t<>>
    inline constexpr bool HasContext = false;

    template<typename T>
    inline constexpr bool HasContext<T, std::void_t<typename T::Context>> = true;

    struct NoContextResource {
      struct Context { };
    };
  }

  template<typename T>
  struct ResourceContextTraits {
    using Type = typename std::conditional_t<details::HasContext<T>, T, details::NoContextResource>::Context;
  };

  template<typename T>
  using ResourceContext = typename ResourceContextTraits<T>::Type;

}

#endif // GF_RESOURCE_CONTEXT_H
