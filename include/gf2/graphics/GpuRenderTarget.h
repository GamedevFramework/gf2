// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_RENDER_TARGET_H
#define GF_GPU_RENDER_TARGET_H

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API GpuRenderTarget {
  public:
    GpuRenderTarget() = default;

  private:
    friend class RenderManager;
    friend class GpuCommandBuffer;
    friend class GpuTexture;

    GpuRenderTarget(SDL_GPUTexture* texture)
    : m_texture(texture)
    {
    }

    SDL_GPUTexture* m_texture = nullptr;
  };

}

#endif // GF_GPU_RENDER_TARGET_H
