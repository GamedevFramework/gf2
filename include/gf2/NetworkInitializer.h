// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_NETWORK_INITIALIZER_H
#define GF_NETWORK_INITIALIZER_H

#include "NetworkApi.h"

namespace gf {

  class GF_NETWORK_API NetworkInitializer {
  public:

#ifdef _WIN32
    NetworkInitializer();
    ~NetworkInitializer();

    NetworkInitializer(const NetworkInitializer& /* unused */) = delete;
    NetworkInitializer(NetworkInitializer&& /* unused */) noexcept;

    NetworkInitializer& operator=(const NetworkInitializer& /* unused */) = delete;
    NetworkInitializer& operator=(NetworkInitializer&& /* unused */) noexcept;
#endif

  };

}

#endif // GF_NETWORK_INITIALIZER_H
