// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_MANAGER_H
#define GF_RENDER_MANAGER_H

#include <array>
#include <thread>
#include <vector>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/Span.h>

#include "GpuCommandBuffer.h"
#include "GpuDevice.h"
#include "GpuTransferBuffer.h"
#include "GpuTexture.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "Window.h"

namespace gf {

  class GF_GRAPHICS_API RenderManager {
  public:
    RenderManager(Window* window);
    RenderManager(const RenderManager&) = delete;
    RenderManager(RenderManager&&) = default;
    ~RenderManager();

    RenderManager& operator=(const RenderManager&) = delete;
    RenderManager& operator=(RenderManager&&) = default;

    Vec2I surface_size() const;
    GpuTextureFormat surface_format();

    GpuCommandBuffer begin_command_buffer();
    void end_command_buffer(GpuCommandBuffer buffer);

    GpuCopyPass current_copy_pass();
    void defer_release_transfer_buffer(GpuTransferBuffer buffer);

    void prepare_asynchronous_load();
    void begin_asynchronous_load();
    void end_asynchronous_load();
    void finish_asynchronous_load();

    RenderTarget current_render_target() const;

    void wait_idle();

    GpuDevice* device();

  private:
    static constexpr std::size_t FramesInFlight = 3;

    SDL_Window* m_window = nullptr; // non-owning
    std::thread::id m_thread_id;

    // device stuff

    GpuDevice m_device;

    // swapchain stuff

    SDL_GPUTexture* m_swapchain_texture = nullptr;
    Vec2I m_swapchain_texture_size = { 0, 0 };

    // commands stuff

    uint32_t m_current_frame = 0;
    std::array<SDL_GPUCommandBuffer*, FramesInFlight> m_command_buffers = {};

    uint32_t m_current_memops = 0;

    struct MemOps {
      SDL_GPUCommandBuffer* command_buffer;
      SDL_GPUCopyPass* copy_pass;
    };

    std::array<MemOps, FramesInFlight> m_memops;

    // synchronization stuff

    using GpuFence = details::GraphicsHandle<SDL_GPUFence, SDL_ReleaseGPUFence>;

    struct RenderSynchronizationObjects {
      GpuFence render_fence;
      GpuFence memops_fence;
    };

    std::array<RenderSynchronizationObjects, FramesInFlight> m_render_synchronization = {};

    // memory stuff

    std::array<std::vector<GpuTransferBuffer>, FramesInFlight> m_transfer_buffers;

    // asynchronous load objects

    bool m_async_loading = false;
    MemOps m_async_memops = {};
    std::vector<GpuTransferBuffer> m_async_staging_buffers;
  };

}

#endif // GF_RENDER_MANAGER_H
