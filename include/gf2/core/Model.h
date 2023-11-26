// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MODEL_H
#define GF_MODEL_H

#include "CoreApi.h"
#include "Time.h"

namespace gf {

  class GF_CORE_API Model {
  public:
    Model() = default;
    Model(const Model&) = delete;
    Model(Model&&) noexcept = default;
    virtual ~Model();

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) noexcept = default;

    virtual void update(Time time);
  };

}

#endif // GF_MODEL_H
