// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ConsoleGraphicsEntity.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  ConsoleGraphicsEntity::ConsoleGraphicsEntity(ConsoleFont* picture_font, ConsoleFont* text_font)
  : m_console(picture_font, text_font)
  {
  }

  ConsoleGraphicsEntity::ConsoleGraphicsEntity(const ConsoleFontResource& resource, ResourceManager* resource_manager)
  : m_console(resource, resource_manager)
  {
  }

  ConsoleGraphicsEntity::ConsoleGraphicsEntity(const MixedConsoleFontResource& resource, ResourceManager* resource_manager)
  : m_console(resource, resource_manager)
  {
  }

  void ConsoleGraphicsEntity::render(RenderRecorder& recorder)
  {
    // common
    RenderObject object = {};
    object.priority = priority();
    object.transform = compute_matrix(m_console.bounds());

    // background
    object.geometry = m_console.background_geometry();
    recorder.record(object);

    // foreground picture
    object.geometry = m_console.foreground_picture_geometry();
    recorder.record(object);

    // foreground text
    object.geometry = m_console.foreground_text_geometry();
    recorder.record(object);
  }

}
