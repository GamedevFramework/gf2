// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_VERTEX_H
#define GF_VERTEX_H

#include "Color.h"
#include "Vec2.h"
#include "VertexInput.h"


namespace gf {

  struct SimpleVertex {
    Vec2F location;
    Color color;

    static VertexInput compute_input();
  };

  struct Vertex {
    Vec2F location;
    Vec2F tex_coords;
    Color color;

    static VertexInput compute_input();
  };

}

#endif // GF_VERTEX_H
