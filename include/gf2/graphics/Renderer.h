// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RENDERER_H
#define GF_RENDERER_H

#include <optional>
#include <type_traits>
#include <vector>

#include <vk_mem_alloc.h>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/Span.h>

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Descriptor.h"
#include "GraphicsApi.h"
#include "Pipeline.h"
#include "RenderTarget.h"
#include "Vulkan.h"
#include "Window.h"

namespace gf {

  namespace details {

    template<typename T>
    uint64_t to_debug_handle(T handle)
    {
      if constexpr (std::is_pointer_v<T>) {
        return static_cast<uint64_t>(reinterpret_cast<std::uintptr_t>(handle)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
      } else {
        static_assert(std::is_integral_v<T>);
        return static_cast<uint64_t>(handle);
      }
    }

  }

  class GF_GRAPHICS_API BasicRenderer {
  public:
    BasicRenderer(Window* window);
    BasicRenderer(const BasicRenderer&) = delete;
    BasicRenderer(BasicRenderer&& other) noexcept;
    ~BasicRenderer();

    BasicRenderer& operator=(const BasicRenderer&) = delete;
    BasicRenderer& operator=(BasicRenderer&& other) noexcept;

    Vec2I surface_size() const;

    std::optional<CommandBuffer> begin_command_buffer();
    void end_command_buffer(CommandBuffer buffer);

    MemoryCommandBuffer current_memory_command_buffer();
    void defer_release_staging_buffer(StagingBufferReference buffer);

    Descriptor allocate_descriptor_for_layout(const DescriptorLayout* layout) const;
    RenderTarget current_render_target() const;

    void wait_idle() const;

  protected:
    void recreate_swapchain();
    void recreate_swapchain(Vec2I size);

  private:
    friend class Buffer;
    friend class DescriptorLayout;
    friend class PipelineBuilder;
    friend class Shader;
    friend class Texture;

    void construct_device();
    void destroy_device();
    void construct_allocator();
    void destroy_allocator();
    void construct_swapchain(Vec2I size);
    void destroy_swapchain();
    void construct_commands();
    void destroy_commands();
    void construct_synchronization();
    void destroy_synchronization();
    void construct_descriptors();
    void destroy_descriptors();

    SDL_Window* m_window = nullptr; // non-owning

    // device stuff

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_messenger = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    uint32_t m_graphics_queue_index = 0;
    VkQueue m_graphics_queue = VK_NULL_HANDLE;
    uint32_t m_present_queue_index = 0;
    VkQueue m_present_queue = VK_NULL_HANDLE;

    VmaAllocator m_allocator = nullptr;

    // swapchain stuff

    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    uint32_t m_image_count = 0;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    VkExtent2D m_extent = { 0, 0 };

    uint32_t m_current_image = 0;

    std::vector<VkImage> m_swapchain_images;
    std::vector<VkImageView> m_swapchain_image_views;

    // commands stuff

    uint32_t m_current_frame = 0;

    VkCommandPool m_command_pool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_command_buffers;

    uint32_t m_current_memops = 0;

    VkCommandPool m_memops_command_pool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_memops_command_buffers;

    // synchronization stuff

    struct RenderSynchronizationObjects {
      VkSemaphore present_semaphore = VK_NULL_HANDLE;
      VkSemaphore render_semaphore = VK_NULL_HANDLE;
      VkFence render_fence = VK_NULL_HANDLE;
      VkFence memops_fence = VK_NULL_HANDLE;
    };

    std::vector<RenderSynchronizationObjects> m_render_synchronization;

    // memory stuff

    std::vector<std::vector<StagingBufferReference>> m_staging_buffers;

    // descriptor set stuff

    std::vector<VkDescriptorPool> m_descriptor_pools;
  };

  class GF_GRAPHICS_API Renderer : public BasicRenderer {
  public:
    Renderer(Window* window);

    void update_surface_size(Vec2I size);

    const DescriptorLayout* camera_descriptor() const
    {
      return &m_camera_descriptor;
    }

    const DescriptorLayout* sampler_descriptor() const
    {
      return &m_sampler_descriptor;
    }

    const Pipeline* simple_pipeline() const
    {
      return &m_simple_pipeline;
    }

    const Pipeline* default_pipeline() const
    {
      return &m_default_pipeline;
    }

    const Pipeline* triangle_strip_pipeline() const
    {
      return &m_triangle_strip_pipeline;
    }

    const Pipeline* text_pipeline() const
    {
      return &m_text_pipeline;
    }

    const Pipeline* fullscreen_pipeline() const
    {
      return &m_fullscreen_pipeline;
    }

  private:
    void build_default_pipelines();

    DescriptorLayout m_camera_descriptor;
    DescriptorLayout m_sampler_descriptor;

    Pipeline m_simple_pipeline;
    Pipeline m_default_pipeline;
    Pipeline m_triangle_strip_pipeline;
    Pipeline m_text_pipeline;
    Pipeline m_fullscreen_pipeline;
  };

}

#endif // GF_RENDERER_H
