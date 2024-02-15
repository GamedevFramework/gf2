// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/CurveEntity.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  CurveEntity::CurveEntity(const CurveData& data, RenderManager* render_manager)
  : m_curve(data, render_manager)
  {
  }

  void CurveEntity::render(RenderRecorder& recorder)
  {
    auto curve_geometry = m_curve.geometry();
    auto model_matrix = compute_matrix(m_curve.bounds());

    if (curve_geometry.outline.has_value()) {
      RenderObject object = {};
      object.priority = priority();
      object.geometry = curve_geometry.outline.value();
      object.transform = model_matrix;
      recorder.record(object);
    }

    RenderObject object = {};
    object.priority = priority();
    object.geometry = curve_geometry.curve;
    object.transform = model_matrix;
    recorder.record(object);
  }

}
