// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_VERTEX_H
#define GF_VERTEX_H

#include <gf2/core/Color.h>
#include <gf2/core/Vec2.h>

#include "VertexInput.h"

namespace gf {

  struct SimpleVertex {
    Vec2F location = { 0.0f, 0.0f };
    Color color = White;

    static VertexInput compute_input();
  };

  struct Vertex {
    Vec2F location = { 0.0f, 0.0f };
    Vec2F tex_coords = { 0.0f, 0.0f };
    Color color = White;

    static VertexInput compute_input();
  };

}

#endif // GF_VERTEX_H
