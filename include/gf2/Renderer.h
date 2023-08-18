#ifndef GF_RENDERER_H
#define GF_RENDERER_H

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

#include "Buffer.h"
#include "Color.h"
#include "CommandBuffer.h"
#include "GraphicsApi.h"
#include "RenderTarget.h"
#include "Span.h"
#include "Window.h"

namespace gf {

  class GF_GRAPHICS_API Renderer {
  public:
    Renderer(Window& window);
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&& other) noexcept;
    ~Renderer();

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&& other) noexcept;

    std::optional<CommandBuffer> begin_command_buffer();
    void end_command_buffer(CommandBuffer buffer);

    RenderTarget current_render_target();

    Buffer allocate_buffer(BufferType type, BufferUsage usage, std::size_t size, std::size_t member_size, const void* data);

    template<typename T>
    Buffer allocate_buffer(BufferType type, BufferUsage usage, const T* data, std::size_t size) {
      return allocate_buffer(type, usage, size, sizeof(T), static_cast<const void*>(data));
    }

    void recreate_swapchain();
    void wait_idle();

  private:
    friend class Shader;
    friend class PipelineBuilder;

    void construct_device();
    void destroy_device();
    void construct_allocator();
    void destroy_allocator();
    void construct_swapchain();
    void destroy_swapchain();
    void construct_commands();
    void destroy_commands();
    void construct_synchronization();
    void destroy_synchronization();

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

    // synchronization stuff

    struct RenderSynchronizationObjects {
      VkSemaphore present_semaphore = VK_NULL_HANDLE;
      VkSemaphore render_semaphore = VK_NULL_HANDLE;
      VkFence render_fence = VK_NULL_HANDLE;
    };

    std::vector<RenderSynchronizationObjects> m_render_synchronization;

  };

}

#endif // GF_RENDERER_H
