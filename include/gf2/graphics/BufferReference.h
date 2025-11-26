// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BUFFER_REFERENCE_H
#define GF_BUFFER_REFERENCE_H

#include "GraphicsApi.h"

namespace gf {
  class Buffer;

  class GF_GRAPHICS_API BufferReference {
  public:
    BufferReference(Buffer& buffer);

  private:
    friend class MemoryCommandBuffer;
    friend class Buffer;
    friend class Texture;

  };

}

#endif // GF_BUFFER_REFERENCE_H
