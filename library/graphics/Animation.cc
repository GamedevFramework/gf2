// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Animation.h>
// clang-format on

#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {

    constexpr std::size_t IndicesPerAnimationFrame = 6;

  }

  Animation::Animation(std::vector<const Texture*> textures, const AnimationData& data, Renderer* renderer)
  : m_textures(std::move(textures))
  , m_properties(data.properties)
  {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    for (const auto& frame_data : data.frames) {
      assert(frame_data.texture_index < m_textures.size());

      AnimationFrame frame = {};
      frame.texture_index = frame_data.texture_index;
      frame.duration = frame_data.duration;
      m_frames.push_back(frame);

      assert(vertices.size() < UINT16_MAX);
      auto index = static_cast<uint16_t>(vertices.size());

      const Vec2F size = m_textures[frame_data.texture_index]->size() * frame_data.texture_region.size();
      const RectF bounds = RectF::from_size(size);

      vertices.push_back({ bounds.position_at(Orientation::NorthEast), frame_data.texture_region.position_at(Orientation::NorthEast), data.color });
      vertices.push_back({ bounds.position_at(Orientation::SouthEast), frame_data.texture_region.position_at(Orientation::SouthEast), data.color });
      vertices.push_back({ bounds.position_at(Orientation::NorthWest), frame_data.texture_region.position_at(Orientation::NorthWest), data.color });
      vertices.push_back({ bounds.position_at(Orientation::SouthWest), frame_data.texture_region.position_at(Orientation::SouthWest), data.color });

      // first triangle
      indices.push_back(index);
      indices.push_back(index + 1);
      indices.push_back(index + 2);

      // second triangle
      indices.push_back(index + 2);
      indices.push_back(index + 1);
      indices.push_back(index + 3);

      m_bounds.extend_to(bounds);
    }

    if (m_frames.empty()) {
      return;
    }

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), renderer);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), renderer);

    reset();
  }

  void Animation::update(Time time)
  {
    if (m_frames.empty()) {
      return;
    }

    m_current_time -= time;

    while (m_current_time < Time()) {
      if (m_current_frame + 1 >= m_frames.size() && !m_properties.test(AnimationProperties::Loop)) {
        return;
      }

      m_current_frame = (m_current_frame + 1) % m_frames.size();
      m_current_time += m_frames[m_current_frame].duration;
    }
  }

  void Animation::reset()
  {
    m_current_frame = 0;

    if (m_frames.empty()) {
      m_current_time = Time();
    } else {
      m_current_time = m_frames.front().duration;
    }
  }

  bool Animation::finished() const
  {
    return !m_properties.test(AnimationProperties::Loop) && m_current_time <= Time() && m_current_frame + 1 >= m_frames.size();
  }

  RenderGeometry Animation::geometry() const
  {
    RenderGeometry geometry;
    geometry.vertices = vertices();
    geometry.indices = indices();
    geometry.count = index_count();
    geometry.first = first_index();
    geometry.texture = texture();
    return geometry;
  }

  const Texture* Animation::texture() const
  {
    if (m_frames.empty()) {
      return nullptr;
    }

    return m_textures[m_frames[m_current_frame].texture_index];
  }

  const Buffer* Animation::vertices() const
  {
    if (m_frames.empty()) {
      return nullptr;
    }

    return &m_vertices;
  }

  const Buffer* Animation::indices() const
  {
    if (m_frames.empty()) {
      return nullptr;
    }

    return &m_indices;
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  std::size_t Animation::index_count() const
  {
    return IndicesPerAnimationFrame;
  }

  std::size_t Animation::first_index() const
  {
    return m_current_frame * IndicesPerAnimationFrame;
  }

}
