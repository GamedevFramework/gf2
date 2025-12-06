// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Vertex.h>

namespace gf {

  GpuVertexInput Vertex::compute_input()
  {
    GpuVertexInput input;

    input.buffer_descriptions.push_back({ 0, sizeof(Vertex) });

    input.attributes.push_back({ 0, 0, GpuVertexElementFormat::Float2, offsetof(Vertex, location) });
    input.attributes.push_back({ 1, 0, GpuVertexElementFormat::Float2, offsetof(Vertex, tex_coords) });
    input.attributes.push_back({ 2, 0, GpuVertexElementFormat::Float4, offsetof(Vertex, color) });

    return input;
  }

}
