// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_TARGET_H
#define GF_RENDER_TARGET_H

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API RenderTarget {
  public:
    RenderTarget() = default;

  private:
    friend class RenderManager;
    friend class CommandBuffer;
    friend class Texture;

    RenderTarget(SDL_GPUTexture* texture)
    : m_texture(texture)
    {
    }

    SDL_GPUTexture* m_texture = nullptr;
  };

}

#endif // GF_RENDER_TARGET_H
