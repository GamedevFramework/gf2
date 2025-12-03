// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_MANAGER_H
#define GF_RENDER_MANAGER_H

#include <mutex>
#include <optional>
#include <thread>
#include <type_traits>
#include <vector>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/Span.h>

#include "Buffer.h"
#include "Descriptor.h"
#include "GpuCommandBuffer.h"
#include "GpuDevice.h"
#include "GraphicsApi.h"
#include "RenderPipeline.h"
#include "RenderTarget.h"
#include "TransferBuffer.h"
#include "Window.h"

namespace gf {

  class GF_GRAPHICS_API RenderManager {
  public:
    RenderManager(Window* window);

    void update_surface_size(Vec2I size);
    Vec2I surface_size() const;

    std::optional<GpuCommandBuffer> begin_command_buffer();
    void end_command_buffer(GpuCommandBuffer buffer);

    GpuCopyPass current_copy_pass();
    void defer_release_transfer_buffer(TransferBuffer buffer);

    void prepare_asynchronous_load();
    void begin_asynchronous_load();
    void end_asynchronous_load();
    void finish_asynchronous_load();

    Descriptor allocate_descriptor_for_layout(const DescriptorLayout* layout) const;
    RenderTarget current_render_target() const;

    void wait_idle() const;

    GpuDevice* device();

  private:
    friend class Buffer;
    friend class DescriptorLayout;
    friend class RenderPipelineBuilder;
    friend class RenderPipelineLayoutBuilder;
    friend class Shader;
    friend class Texture;

    void finish_staging_buffers();

    SDL_Window* m_window = nullptr; // non-owning
    std::thread::id m_thread_id;

    // device stuff

    GpuDevice m_device;

    // swapchain stuff

    // commands stuff

    uint32_t m_current_frame = 0;

    uint32_t m_current_memops = 0;

    // synchronization stuff

    struct RenderSynchronizationObjects {
      // VkFence render_fence = VK_NULL_HANDLE;
      // VkFence memops_fence = VK_NULL_HANDLE;
    };

    std::vector<RenderSynchronizationObjects> m_render_synchronization;

    // memory stuff

    std::vector<std::vector<TransferBuffer>> m_transfer_buffers;

    // descriptor set stuff


    // asynchronous load objects

    bool m_async_loading = false;
    std::vector<TransferBuffer> m_async_staging_buffers;
  };

}

#endif // GF_RENDER_MANAGER_H
