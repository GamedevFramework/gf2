// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_TYPE_TRAITS_H
#define GF_TYPE_TRAITS_H

namespace gf {

  template<typename Data, typename Archive>
  using MaybeConst = typename Archive::template Constness<Data>;

}

#endif // GF_TYPE_TRAITS_H
