// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_RENDER_MANAGER_H
#define GF_RENDER_MANAGER_H

#include <mutex>
#include <optional>
#include <thread>
#include <type_traits>
#include <vector>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/Span.h>

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Descriptor.h"
#include "GraphicsApi.h"
#include "RenderPipeline.h"
#include "RenderTarget.h"
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

  class GF_GRAPHICS_API RenderManager {
  public:
    RenderManager(Window* window);
    RenderManager(const RenderManager&) = delete;
    RenderManager(RenderManager&& other) noexcept;
    ~RenderManager();

    RenderManager& operator=(const RenderManager&) = delete;
    RenderManager& operator=(RenderManager&& other) noexcept;

    void update_surface_size(Vec2I size);
    Vec2I surface_size() const;

    std::optional<CommandBuffer> begin_command_buffer();
    void end_command_buffer(CommandBuffer buffer);

    MemoryCommandBuffer current_memory_command_buffer();
    void defer_release_staging_buffer(StagingBufferReference buffer);

    void prepare_asynchronous_load();
    void begin_asynchronous_load();
    void end_asynchronous_load();
    void finish_asynchronous_load();

    Descriptor allocate_descriptor_for_layout(const DescriptorLayout* layout) const;
    RenderTarget current_render_target() const;

    void wait_idle() const;

  protected:
    void recreate_swapchain();
    void recreate_swapchain(Vec2I size);

  private:
    friend class Buffer;
    friend class DescriptorLayout;
    friend class RenderPipelineBuilder;
    friend class RenderPipelineLayoutBuilder;
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
    void construct_async_objects();
    void destroy_async_objects();

    void finish_staging_buffers();

    SDL_Window* m_window = nullptr; // non-owning
    std::thread::id m_thread_id;

    // device stuff

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_messenger = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;

    std::mutex m_queue_mutex;

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

    // asynchronous load objects

    bool m_async_loading = false;
    VkCommandPool m_async_command_pool = VK_NULL_HANDLE;
    VkCommandBuffer m_async_command_buffer = VK_NULL_HANDLE;
    VkFence m_async_fence = VK_NULL_HANDLE;
    std::vector<StagingBufferReference> m_async_staging_buffers;
  };

}

#endif // GF_RENDER_MANAGER_H
