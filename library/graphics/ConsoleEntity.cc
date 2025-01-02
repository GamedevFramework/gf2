// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ConsoleEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  ConsoleEntity::ConsoleEntity(const ConsoleFont* font, const ConsoleBuffer& buffer, RenderManager* render_manager)
  : m_console(font, buffer, render_manager)
  {
  }

  ConsoleEntity::ConsoleEntity(const ConsoleResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_console(resource, render_manager, resource_manager)
  {
  }

  void ConsoleEntity::render(RenderRecorder& recorder)
  {
    // common
    RenderObject object = {};
    object.priority = priority();
    object.transform = compute_matrix(m_console.bounds());

    // background
    object.geometry = m_console.background_geometry();
    recorder.record(object);

    // foreground
    object.geometry = m_console.foreground_geometry();
    recorder.record(object);
  }

}
