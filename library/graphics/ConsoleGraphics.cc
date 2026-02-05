// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/ConsoleGraphics.h>

#include <gf2/core/Log.h>
#include <gf2/core/ResourceManager.h>

#include "gf2/core/Console.h"
#include "gf2/graphics/RawGeometry.h"

namespace gf {

  ConsoleGraphics::ConsoleGraphics()
  : m_background_vertices(GpuBufferUsage::Vertex)
  , m_background_indices(GpuBufferUsage::Index)
  , m_foreground_picture_vertices(GpuBufferUsage::Vertex)
  , m_foreground_picture_indices(GpuBufferUsage::Index)
  , m_foreground_text_vertices(GpuBufferUsage::Vertex)
  , m_foreground_text_indices(GpuBufferUsage::Index)
  {
    m_background_vertices.set_debug_name("[gf2] Console Background Vertex Buffer");
    m_background_indices.set_debug_name("[gf2] Console Background Index Buffer");
    m_foreground_picture_vertices.set_debug_name("[gf2] Console Foreground Picture Vertex Buffer");
    m_foreground_picture_indices.set_debug_name("[gf2] Console Foreground Picture Index Buffer");
    m_foreground_text_vertices.set_debug_name("[gf2] Console Foreground Text Vertex Buffer");
    m_foreground_text_indices.set_debug_name("[gf2] Console Foreground Text Index Buffer");
  }

  ConsoleGraphics::ConsoleGraphics(ConsoleFont* picture_font, ConsoleFont* text_font)
  : ConsoleGraphics()
  {
    m_picture_font = picture_font;
    m_text_font = text_font;

    if (m_picture_font != nullptr && m_text_font != nullptr) {
      const Vec2I picture_font_size = m_picture_font->character_size();
      const Vec2I text_font_size = m_text_font->character_size();

      if (picture_font_size.w * text_font_size.h != 2 * picture_font_size.h * text_font_size.w) {
        Log::warning("Picture font ratio and text font ratio don't match.");
      }
    }
  }

  ConsoleGraphics::ConsoleGraphics(const ConsoleFontResource& resource, ResourceManager* resource_manager)
  : ConsoleGraphics(resource_manager->get<ConsoleFont>(resource.console_font))
  {
  }

  ConsoleGraphics::ConsoleGraphics(const MixedConsoleFontResource& resource, ResourceManager* resource_manager)
  : ConsoleGraphics(resource_manager->get<ConsoleFont>(resource.picture.console_font), resource_manager->get<ConsoleFont>(resource.text.console_font))
  {
  }

  void ConsoleGraphics::update(const Console& console, RenderManager* render_manager)
  {
    if (m_picture_font != nullptr) {
      update_background(console, render_manager);
      update_foreground_picture(console, render_manager);

      if (m_text_font != nullptr) {
        update_foreground_text(console, render_manager);
      }
    }

    m_bounds = RectF::from_size(console.size() * m_picture_font->character_size());
  }

  RenderGeometry ConsoleGraphics::background_geometry()
  {
    RenderGeometry geometry;
    geometry.vertices = &m_background_vertices.buffer();
    geometry.indices = &m_background_indices.buffer();
    geometry.count = m_background_indices.buffer().member_count();
    return geometry;
  }

  RenderGeometry ConsoleGraphics::foreground_picture_geometry()
  {
    RenderGeometry geometry;
    geometry.vertices = &m_foreground_picture_vertices.buffer();
    geometry.indices = &m_foreground_picture_indices.buffer();
    geometry.count = m_foreground_picture_indices.buffer().member_count();
    geometry.texture = m_picture_font->texture();
    return geometry;
  }

  RenderGeometry ConsoleGraphics::foreground_text_geometry()
  {
    if (m_text_font == nullptr) {
      return {};
    }

    RenderGeometry geometry;
    geometry.vertices = &m_foreground_text_vertices.buffer();
    geometry.indices = &m_foreground_text_indices.buffer();
    geometry.count = m_foreground_text_indices.buffer().member_count();
    geometry.texture = m_text_font->texture();
    return geometry;
  }

  void ConsoleGraphics::update_background(const Console& console, RenderManager* render_manager)
  {
    RawGeometry geometry;

    const Vec2I size = m_picture_font->character_size();
    const Vec2I offset = dirx(size.w / 2);
    const Vec2I half_size = { size.w / 2, size.h };

    const Vec2F texture_coordinates = { 0.0f, 0.0f };

    for (const Vec2I position : console.position_range()) {
      const ConsoleCell& cell = console(position);

      if (cell.mode == ConsoleMode::Picture || cell.parts[0].background == cell.parts[1].background) {
        const RectF bounds = RectF::from_position_size(position * size, size);
        const Color color = srgb_to_linear(cell.parts[0].background);

        auto index = static_cast<uint16_t>(geometry.vertices.size());

        geometry.vertices.push_back({ bounds.position_at(Orientation::NorthEast), texture_coordinates, color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::SouthEast), texture_coordinates, color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::NorthWest), texture_coordinates, color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::SouthWest), texture_coordinates, color });

        // first triangle
        geometry.indices.push_back(index);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 2);

        // second triangle
        geometry.indices.push_back(index + 2);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 3);
      } else {

        for (int32_t part : { 0, 1 }) {
          const RectF bounds = RectF::from_position_size(position * size + part * offset, half_size);
          const Color color = srgb_to_linear(cell.parts[part].background);

          auto index = static_cast<uint16_t>(geometry.vertices.size());

          geometry.vertices.push_back({ bounds.position_at(Orientation::NorthEast), texture_coordinates, color });
          geometry.vertices.push_back({ bounds.position_at(Orientation::SouthEast), texture_coordinates, color });
          geometry.vertices.push_back({ bounds.position_at(Orientation::NorthWest), texture_coordinates, color });
          geometry.vertices.push_back({ bounds.position_at(Orientation::SouthWest), texture_coordinates, color });

          // first triangle
          geometry.indices.push_back(index);
          geometry.indices.push_back(index + 1);
          geometry.indices.push_back(index + 2);

          // second triangle
          geometry.indices.push_back(index + 2);
          geometry.indices.push_back(index + 1);
          geometry.indices.push_back(index + 3);
        }

      }
    }

    m_background_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_background_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

  void ConsoleGraphics::update_foreground_picture(const Console& console, RenderManager* render_manager)
  {
    assert(m_picture_font != nullptr);

    RawGeometry geometry;
    const Vec2I size = m_picture_font->character_size();

    for (const Vec2I position : console.position_range()) {
      const ConsoleCell& cell = console(position);

      if (cell.mode != ConsoleMode::Picture) {
        continue;
      }

      const RectI bounds = RectI::from_position_size(position * size, size);
      const RectF texture_region = m_picture_font->texture_region(cell.parts[0].character);
      const Color color = srgb_to_linear(cell.parts[0].foreground);

      auto index = static_cast<uint16_t>(geometry.vertices.size());

      geometry.vertices.push_back({ bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color });
      geometry.vertices.push_back({ bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color });
      geometry.vertices.push_back({ bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color });
      geometry.vertices.push_back({ bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color });

      // first triangle
      geometry.indices.push_back(index);
      geometry.indices.push_back(index + 1);
      geometry.indices.push_back(index + 2);

      // second triangle
      geometry.indices.push_back(index + 2);
      geometry.indices.push_back(index + 1);
      geometry.indices.push_back(index + 3);
    }

    m_foreground_picture_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_foreground_picture_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

  void ConsoleGraphics::update_foreground_text(const Console& console, RenderManager* render_manager)
  {
    assert(m_picture_font != nullptr);
    assert(m_text_font != nullptr);

    RawGeometry geometry;

    const Vec2I size = m_picture_font->character_size();
    const Vec2I offset = dirx(size.w / 2);
    const Vec2I half_size = { size.w / 2, size.h };

    for (const Vec2I position : console.position_range()) {
      const ConsoleCell& cell = console(position);

      if (cell.mode != ConsoleMode::Text) {
        continue;
      }

      for (int32_t part : { 0, 1 }) {
        const RectI bounds = RectI::from_position_size(position * size + part *offset, half_size);
        const RectF texture_region = m_text_font->texture_region(cell.parts[part].character);
        const Color color = srgb_to_linear(cell.parts[part].foreground);

        auto index = static_cast<uint16_t>(geometry.vertices.size());

        geometry.vertices.push_back({ bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color });
        geometry.vertices.push_back({ bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color });

        // first triangle
        geometry.indices.push_back(index);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 2);

        // second triangle
        geometry.indices.push_back(index + 2);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 3);
      }

    }

    m_foreground_text_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_foreground_text_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

}
