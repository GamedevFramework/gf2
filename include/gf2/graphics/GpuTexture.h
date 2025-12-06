// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GPU_TEXTURE_H
#define GF_GPU_TEXTURE_H

#include <filesystem>
#include <string>
#include <type_traits>

#include <SDL3/SDL_gpu.h>

#include <gf2/core/Bitmap.h>
#include <gf2/core/Flags.h>
#include <gf2/core/Image.h>

#include "GraphicsApi.h"
#include "GraphicsHandle.h"
#include "RenderTarget.h"

namespace gf {
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuTextureFormat : std::underlying_type_t<SDL_GPUTextureFormat> {
    Undefined = SDL_GPU_TEXTUREFORMAT_INVALID,
    R8G8B8A8_SNorm = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM,
    R8G8B8A8_UNorm = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
    R32G32B32A32_Float = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT,
    R8_UNorm = SDL_GPU_TEXTUREFORMAT_R8_UNORM,
    R32G32_Float = SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT,
    RenderManager = std::numeric_limits<std::underlying_type_t<SDL_GPUTextureFormat>>::max(),
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class GpuTextureUsage : SDL_GPUTextureUsageFlags {
    Sampler = SDL_GPU_TEXTUREUSAGE_SAMPLER,
    ColorTarget = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
    DepthStencilTarget = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
    GraphicsStorageRead = SDL_GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ,
    ComputeStorageRead = SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ,
    ComputeStorageWrite = SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE,
    ComputeStorageSimultaneousReadWrite = SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE,
  };

  template<>
  struct EnableBitmaskOperators<GpuTextureUsage> : std::true_type {
  };

  class GF_GRAPHICS_API GpuTexture {
  public:
    using Context = RenderManager*;

    GpuTexture() = default;
    GpuTexture(const std::filesystem::path& filename, RenderManager* render_manager);
    GpuTexture(const Image& image, RenderManager* render_manager);
    GpuTexture(const Bitmap& bitmap, RenderManager* render_manager);
    GpuTexture(Vec2I size, RenderManager* render_manager);
    GpuTexture(Vec2I size, Flags<GpuTextureUsage> usage, GpuTextureFormat format, RenderManager* render_manager);

    void set_debug_name(const std::string& name);

    void update(const Image& image, RenderManager* render_manager);
    void update(const Bitmap& bitmap, RenderManager* render_manager);
    void update(std::size_t size, const uint8_t* data, RenderManager* render_manager);

    Vec2I size() const
    {
      return m_image_size;
    }

    RenderTarget as_render_target();

  private:
    friend class Descriptor;

    details::GraphicsHandle<SDL_GPUTexture, SDL_ReleaseGPUTexture> m_texture_handle;
    details::GraphicsHandle<SDL_GPUSampler, SDL_ReleaseGPUSampler> m_sampler_handle;

    Vec2I m_image_size = { 0, 0 };
    Flags<GpuTextureUsage> m_usage = None;
    GpuTextureFormat m_format = GpuTextureFormat::Undefined;
  };

}

#endif // GF_GPU_TEXTURE_H
