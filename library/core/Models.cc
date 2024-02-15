// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Models.h>

#include <cassert>

namespace gf {

  namespace {

    constexpr Time MaxTimeElapsed = gf::seconds(0.5f);

  }

  FixedTimestepModel::FixedTimestepModel(Model* model, Time timestep)
  : m_model(model)
  , m_timestep(timestep)
  {
    assert(model != nullptr);
  }

  void FixedTimestepModel::update(Time time)
  {
    m_elapsed += time;

    if (m_elapsed > MaxTimeElapsed) {
      m_elapsed = MaxTimeElapsed;
    }

    while (m_elapsed > m_timestep) {
      m_model->update(m_timestep);
      m_elapsed -= m_timestep;
    }
  }

}
