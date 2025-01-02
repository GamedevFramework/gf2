// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ModelContainer.h>

#include <gf2/core/Model.h>

namespace gf {

  void ModelContainer::add_model(Model* model)
  {
    m_models.push_back(model);
  }

  void ModelContainer::update(Time time)
  {
    for (auto* model : m_models) {
      model->update(time);
    }
  }

}
