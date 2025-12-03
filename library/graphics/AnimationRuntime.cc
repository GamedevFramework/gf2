// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/AnimationRuntime.h>

#include <limits>

#include <gf2/graphics/Buffer.h>
#include <gf2/graphics/GpuTexture.h>

namespace gf::details {

  AnimationGraphicsRuntime AnimationBuilder::append_data(const std::vector<const GpuTexture*>& textures, const AnimationData& data)
  {
    const Color color = srgb_to_linear(data.color);

    AnimationGraphicsRuntime runtime;

    for (const auto& frame_data : data.frames) {
      assert(frame_data.texture_index < textures.size());

      AnimationGraphicsFrameRuntime frame = {};
      frame.texture_index = frame_data.texture_index;
      frame.offset = indices.size();
      runtime.frames.push_back(frame);

      assert(vertices.size() < std::numeric_limits<uint16_t>::max());
      const auto index = static_cast<uint16_t>(vertices.size());

      const Vec2F size = textures[frame_data.texture_index]->size() * frame_data.texture_region.size();
      const RectF bounds = RectF::from_size(size);

      vertices.push_back({ bounds.position_at(Orientation::NorthEast), frame_data.texture_region.position_at(Orientation::NorthEast), color });
      vertices.push_back({ bounds.position_at(Orientation::SouthEast), frame_data.texture_region.position_at(Orientation::SouthEast), color });
      vertices.push_back({ bounds.position_at(Orientation::NorthWest), frame_data.texture_region.position_at(Orientation::NorthWest), color });
      vertices.push_back({ bounds.position_at(Orientation::SouthWest), frame_data.texture_region.position_at(Orientation::SouthWest), color });

      if (frame_data.flip.test(AnimationFrameFlip::Diagonally)) {
        std::swap(vertices[index + 0].tex_coords, vertices[index + 3].tex_coords);
      }

      if (frame_data.flip.test(AnimationFrameFlip::Horizontally)) {
        std::swap(vertices[index + 0].tex_coords, vertices[index + 2].tex_coords);
        std::swap(vertices[index + 1].tex_coords, vertices[index + 3].tex_coords);
      }

      if (frame_data.flip.test(AnimationFrameFlip::Vertically)) {
        std::swap(vertices[index + 0].tex_coords, vertices[index + 1].tex_coords);
        std::swap(vertices[index + 2].tex_coords, vertices[index + 3].tex_coords);
      }

      // first triangle
      indices.push_back(index);
      indices.push_back(index + 1);
      indices.push_back(index + 2);

      // second triangle
      indices.push_back(index + 2);
      indices.push_back(index + 1);
      indices.push_back(index + 3);

      global_bounds.extend_to(bounds);
    }

    return runtime;
  }

  Buffer AnimationBuilder::create_vertices(RenderManager* render_manager)
  {
    return { BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), render_manager };
  }

  Buffer AnimationBuilder::create_indices(RenderManager* render_manager)
  {
    return { BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), render_manager };
  }

}
