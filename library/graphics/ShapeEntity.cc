// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ShapeEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * ShapeEntity
   */

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

  /*
   * ShapeGroupEntity
   */

  ShapeGroupEntity::ShapeGroupEntity(const ShapeGroupBuffer& buffer, RenderManager* render_manager)
  : m_shape_group(buffer, render_manager)
  {
  }

  void ShapeGroupEntity::render(RenderRecorder& recorder)
  {
    RenderObject object = {};
    object.priority = priority();
    object.geometry = m_shape_group.geometry();
    object.transform = compute_matrix(m_shape_group.bounds());
    recorder.record(object);
  }

}
