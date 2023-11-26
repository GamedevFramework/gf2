// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MODEL_CONTAINER_H
#define GF_MODEL_CONTAINER_H

#include <vector>

#include "CoreApi.h"
#include "Time.h"

namespace gf {
  class Model;

  class GF_CORE_API ModelContainer {
  public:
    void add_model(Model* model);
    void update(Time time);

  private:
    std::vector<Model*> m_models;
  };

}

#endif // GF_MODEL_CONTAINER_H
