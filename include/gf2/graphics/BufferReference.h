// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BUFFER_REFERENCE_H
#define GF_BUFFER_REFERENCE_H

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API BufferReference {
  public:

  private:
    friend class MemoryCommandBuffer;
    friend class Buffer;
    friend class Texture;

  };

  class GF_GRAPHICS_API StagingBufferReference {
  public:

  private:
    friend class RenderManager;
    friend class Buffer;
    friend class Texture;

  };

}

#endif // GF_BUFFER_REFERENCE_H
