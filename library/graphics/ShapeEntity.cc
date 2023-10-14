// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/ShapeEntity.h>
// clang-format on

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  ShapeEntity::ShapeEntity(const Texture* texture, const ShapeData& data, Renderer* renderer)
  : m_shape(texture, data, renderer)
  {
  }

  void ShapeEntity::render(RenderRecorder& recorder)
  {
    auto shape_geometry = m_shape.geometry();
    auto model_matrix = m_transform.compute_matrix(m_shape.bounds());

    if (shape_geometry.outline.has_value()) {
      RenderObject object = {};
      object.priority = priority();
      object.geometry = shape_geometry.outline.value();
      object.transform = model_matrix;
      recorder.record(object);
    }

    RenderObject object = {};
    object.priority = priority();
    object.geometry = shape_geometry.shape;
    object.transform = model_matrix;
    recorder.record(object);
  }

}
