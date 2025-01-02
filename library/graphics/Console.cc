// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Console.h>

#include <gf2/core/ResourceManager.h>

#include "gf2/graphics/RawGeometry.h"

namespace gf {

  Console::Console()
  : m_background_vertices(BufferType::Device, BufferUsage::Vertex)
  , m_background_indices(BufferType::Device, BufferUsage::Index)
  , m_foreground_vertices(BufferType::Device, BufferUsage::Vertex)
  , m_foreground_indices(BufferType::Device, BufferUsage::Index)
  {
    m_background_vertices.set_debug_name("[gf2] Console Background Vertex Buffer");
    m_background_indices.set_debug_name("[gf2] Console Background Index Buffer");
    m_foreground_vertices.set_debug_name("[gf2] Console Foreground Vertex Buffer");
    m_foreground_indices.set_debug_name("[gf2] Console Foreground Index Buffer");
  }

  Console::Console(const ConsoleFont* font, const ConsoleBuffer& buffer, RenderManager* render_manager)
  : Console()
  {
    m_font = font;
    update(buffer, render_manager);
  }

  Console::Console(const ConsoleResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Console(resource_manager->get<ConsoleFont>(resource.console_font), resource.buffer, render_manager)
  {
  }

  void Console::update(const ConsoleBuffer& buffer, RenderManager* render_manager)
  {
    if (m_font == nullptr) {
      return;
    }

    update_background(buffer, render_manager);
    update_foreground(buffer, render_manager);

    m_bounds = RectF::from_size(buffer.cells.size() * m_font->character_size());
  }

  RenderGeometry Console::background_geometry() const
  {
    RenderGeometry geometry;
    geometry.vertices = &m_background_vertices.buffer();
    geometry.indices = &m_background_indices.buffer();
    geometry.size = m_background_indices.buffer().size();
    return geometry;
  }

  RenderGeometry Console::foreground_geometry() const
  {
    RenderGeometry geometry;
    geometry.vertices = &m_foreground_vertices.buffer();
    geometry.indices = &m_foreground_indices.buffer();
    geometry.size = m_foreground_indices.buffer().size();
    geometry.texture = m_font->texture();
    return geometry;
  }

  void Console::update_background(const ConsoleBuffer& buffer, RenderManager* render_manager)
  {
    RawGeometry geometry;

    for (auto position : buffer.cells.position_range()) {
      const auto& cell = buffer.cells(position);

      const RectF bounds = RectF::from_position_size(position * m_font->character_size(), m_font->character_size());
      const Vec2F texture_coordinates = { 0.0f, 0.0f };
      const Color color = srgb_to_linear(cell.background);

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

    m_background_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_background_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

  void Console::update_foreground(const ConsoleBuffer& buffer, RenderManager* render_manager)
  {
    RawGeometry geometry;

    for (auto position : buffer.cells.position_range()) {
      const auto& cell = buffer.cells(position);

      const Vec2I character_size = m_font->character_size();
      const RectI bounds = RectI::from_position_size(position * character_size, character_size);
      const RectF texture_region = m_font->texture_region(cell.character);
      const Color color = srgb_to_linear(cell.foreground);

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

    m_foreground_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_foreground_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

}
