// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Vertex.h>
// clang-format on

namespace gf {

  VertexInput Vertex::compute_input()
  {
    VertexInput input;

    input.bindings.push_back({ 0, sizeof(Vertex) });

    input.attributes.push_back({ 0, 0, Format::Vec2F, offsetof(Vertex, location) });
    input.attributes.push_back({ 1, 0, Format::Vec2F, offsetof(Vertex, tex_coords) });
    input.attributes.push_back({ 2, 0, Format::Color32F, offsetof(Vertex, color) });

    return input;
  }

}
