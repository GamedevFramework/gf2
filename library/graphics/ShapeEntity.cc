// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/ShapeEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  ShapeEntity::ShapeEntity(const Texture* texture, const ShapeBuffer& buffer, RenderManager* render_manager)
  : m_shape(texture, buffer, render_manager)
  {
  }

  ShapeEntity::ShapeEntity(const ShapeResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_shape(resource, render_manager, resource_manager)
  {
  }

  void ShapeEntity::render(RenderRecorder& recorder)
  {
    auto shape_geometry = m_shape.geometry();
    auto model_matrix = compute_matrix(m_shape.bounds());

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
