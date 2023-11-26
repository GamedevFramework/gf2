// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_MODELS_H
#define GF_MODELS_H

#include "CoreApi.h"
#include "Model.h"

namespace gf {

  class GF_CORE_API FixedTimestepModel : public Model {
  public:
    FixedTimestepModel(Model* model, Time timestep = gf::seconds(1 / 60.0f));

    void update(Time time) override;

  private:
    Model* m_model = nullptr;
    Time m_timestep;
    Time m_elapsed;
  };

}

#endif // GF_MODELS_H
