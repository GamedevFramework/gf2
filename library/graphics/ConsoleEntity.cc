// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ConsoleEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  ConsoleEntity::ConsoleEntity(ConsoleFont* font)
  : m_console(font)
  {
  }

  ConsoleEntity::ConsoleEntity(ConsoleResource& resource, ResourceManager* resource_manager)
  : m_console(resource, resource_manager)
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
