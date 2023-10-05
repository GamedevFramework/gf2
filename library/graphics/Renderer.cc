// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Renderer.h>
// clang-format on

#include <utility>

#include <SDL2/SDL_vulkan.h>
#include <VkBootstrap.h>

#include <gf2/core/Log.h>
#include <gf2/core/Mat3.h>

#include <gf2/graphics/ShaderData.h>
#include <gf2/graphics/Vertex.h>
#include "gf2/graphics/RenderPipeline.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

// #define GF_USE_LLVMPIPE

namespace gf {

  constexpr std::size_t FramesInFlight = 3;

  namespace {
    Log::Level to_log_level(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
    {
      switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
          return Log::Level::Debug;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
          return Log::Level::Info;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
          return Log::Level::Warn;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
          return Log::Level::Error;
        default:
          break;
      }

      return Log::Level::Info;
    }

    VkBool32 debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, [[maybe_unused]] void* user_data)
    {
      Log::print(to_log_level(severity), "<{}> {}", vkb::to_string_message_type(type), data->pMessage);
      return VK_FALSE;
    }

  }

  BasicRenderer::BasicRenderer(Window* window)
  : m_window(window->m_window)
  {
    Vec2I size = {};
    SDL_Vulkan_GetDrawableSize(m_window, &size.w, &size.h);

    construct_device();
    construct_allocator();
    construct_swapchain(size);
    construct_commands();
    construct_synchronization();
    construct_descriptors();
  }

  BasicRenderer::BasicRenderer(BasicRenderer&& other) noexcept
  : m_window(std::exchange(other.m_window, nullptr))
  // device
  , m_instance(std::exchange(other.m_instance, VK_NULL_HANDLE))
  , m_messenger(std::exchange(other.m_messenger, VK_NULL_HANDLE))
  , m_surface(std::exchange(other.m_surface, VK_NULL_HANDLE))
  , m_physical_device(std::exchange(other.m_physical_device, VK_NULL_HANDLE))
  , m_device(std::exchange(other.m_device, VK_NULL_HANDLE))
  , m_graphics_queue_index(other.m_graphics_queue_index)
  , m_graphics_queue(std::exchange(other.m_graphics_queue, VK_NULL_HANDLE))
  , m_present_queue_index(other.m_present_queue_index)
  , m_present_queue(std::exchange(other.m_present_queue, VK_NULL_HANDLE))
  , m_allocator(std::exchange(other.m_allocator, nullptr))
  // swapchain
  , m_swapchain(std::exchange(other.m_swapchain, VK_NULL_HANDLE))
  , m_image_count(other.m_image_count)
  , m_format(other.m_format)
  , m_extent(other.m_extent)
  , m_current_image(other.m_current_image)
  , m_swapchain_images(std::move(other.m_swapchain_images))
  , m_swapchain_image_views(std::move(other.m_swapchain_image_views))
  // commands
  , m_current_frame(other.m_current_frame)
  , m_command_pool(std::exchange(other.m_command_pool, VK_NULL_HANDLE))
  , m_command_buffers(std::move(other.m_command_buffers))
  , m_current_memops(other.m_current_memops)
  , m_memops_command_pool(std::exchange(other.m_memops_command_pool, VK_NULL_HANDLE))
  , m_memops_command_buffers(std::move(other.m_memops_command_buffers))
  // synchronization
  , m_render_synchronization(std::move(other.m_render_synchronization))
  // memory
  , m_staging_buffers(std::move(other.m_staging_buffers))
  // descriptors
  , m_descriptor_pools(std::move(other.m_descriptor_pools))
  {
    other.m_swapchain_images.clear();
    other.m_swapchain_image_views.clear();
    other.m_command_buffers.clear();
    other.m_memops_command_buffers.clear();
    other.m_render_synchronization.clear();
    other.m_staging_buffers.clear();
    other.m_descriptor_pools.clear();
  }

  BasicRenderer::~BasicRenderer()
  {
    vkDeviceWaitIdle(m_device);
    destroy_descriptors();
    destroy_synchronization();
    destroy_commands();
    destroy_swapchain();
    destroy_allocator();
    destroy_device();
  }

  BasicRenderer& BasicRenderer::operator=(BasicRenderer&& other) noexcept
  {
    std::swap(m_window, other.m_window);
    // device
    std::swap(m_instance, other.m_instance);
    std::swap(m_messenger, other.m_messenger);
    std::swap(m_surface, other.m_surface);
    std::swap(m_physical_device, other.m_physical_device);
    std::swap(m_device, other.m_device);
    std::swap(m_graphics_queue_index, other.m_graphics_queue_index);
    std::swap(m_graphics_queue, other.m_graphics_queue);
    std::swap(m_present_queue_index, other.m_present_queue_index);
    std::swap(m_present_queue, other.m_present_queue);
    std::swap(m_allocator, other.m_allocator);
    // swapchain
    std::swap(m_swapchain, other.m_swapchain);
    std::swap(m_image_count, other.m_image_count);
    std::swap(m_format, other.m_format);
    std::swap(m_extent, other.m_extent);
    std::swap(m_current_image, other.m_current_image);
    std::swap(m_swapchain_images, other.m_swapchain_images);
    std::swap(m_swapchain_image_views, other.m_swapchain_image_views);
    // commands
    std::swap(m_current_frame, other.m_current_frame);
    std::swap(m_command_pool, other.m_command_pool);
    std::swap(m_command_buffers, other.m_command_buffers);
    std::swap(m_current_memops, other.m_current_memops);
    std::swap(m_memops_command_pool, other.m_memops_command_pool);
    std::swap(m_memops_command_buffers, other.m_memops_command_buffers);
    // synchronization
    std::swap(m_render_synchronization, other.m_render_synchronization);
    // memory
    std::swap(m_staging_buffers, other.m_staging_buffers);
    // descriptors
    std::swap(m_descriptor_pools, other.m_descriptor_pools);

    return *this;
  }

  Vec2I BasicRenderer::surface_size() const
  {
    return { static_cast<int>(m_extent.width), static_cast<int>(m_extent.height) };
  }

  namespace {

    constexpr VkImageSubresourceRange ImageSubresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

  }

  std::optional<CommandBuffer> BasicRenderer::begin_command_buffer()
  {
    const RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];
    assert(m_current_frame == m_current_memops);

    // synchronize and acquire the next image

    vkWaitForFences(m_device, 1, &sync.render_fence, VK_TRUE, UINT64_MAX);
    const VkResult result = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, sync.render_semaphore, VK_NULL_HANDLE, &m_current_image);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreate_swapchain();
      return std::nullopt;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      Log::error("Failed to acquire swap chain image.");
      throw std::runtime_error("Failed to acquire swap chain image.");
    }

    vkResetFences(m_device, 1, &sync.render_fence);

    // submit memory command buffer

    VkCommandBuffer memops_command_buffer = m_memops_command_buffers[m_current_memops];

    if (vkEndCommandBuffer(memops_command_buffer) != VK_SUCCESS) {
      Log::error("Failed to record command buffer.");
      throw std::runtime_error("Failed to record command buffer.");
    }

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &memops_command_buffer;

    if (vkQueueSubmit(m_graphics_queue, 1, &submit_info, sync.memops_fence) != VK_SUCCESS) {
      Log::error("Failed to submit draw command buffer.");
      throw std::runtime_error("Failed to submit draw command buffer.");
    }

    // begin next memory command buffer

    m_current_memops = (m_current_memops + 1) % FramesInFlight;

    memops_command_buffer = m_memops_command_buffers[m_current_memops];

    vkResetCommandBuffer(memops_command_buffer, 0);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(memops_command_buffer, &begin_info) != VK_SUCCESS) {
      Log::error("Failed to begin recording command buffer.");
      throw std::runtime_error("Failed to begin recording command buffer.");
    }

    // begin render command buffer

    VkCommandBuffer command_buffer = m_command_buffers[m_current_frame];
    vkResetCommandBuffer(command_buffer, 0);

    vkResetDescriptorPool(m_device, m_descriptor_pools[m_current_frame], 0);

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
      Log::error("Failed to begin recording command buffer.");
      throw std::runtime_error("Failed to begin recording command buffer.");
    }

    VkImageMemoryBarrier image_memory_barrier = {};
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    image_memory_barrier.srcQueueFamilyIndex = m_present_queue_index;
    image_memory_barrier.dstQueueFamilyIndex = m_graphics_queue_index;
    image_memory_barrier.image = m_swapchain_images[m_current_image];
    image_memory_barrier.subresourceRange = ImageSubresourceRange;

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

    return CommandBuffer(command_buffer);
  }

  void BasicRenderer::end_command_buffer([[maybe_unused]] CommandBuffer buffer)
  {
    const RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];

    // wait for memops end

    vkWaitForFences(m_device, 1, &sync.memops_fence, VK_TRUE, UINT64_MAX);

    for (auto staging_buffer : m_staging_buffers[m_current_frame]) {
      vmaDestroyBuffer(m_allocator, staging_buffer.m_buffer, staging_buffer.m_allocation);
    }

    m_staging_buffers[m_current_frame].clear();

    vkResetFences(m_device, 1, &sync.memops_fence);

    // submit render command buffer

    VkCommandBuffer command_buffer = m_command_buffers[m_current_frame];
    assert(buffer.m_command_buffer == command_buffer);

    VkImageMemoryBarrier image_memory_barrier = {};
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    image_memory_barrier.srcQueueFamilyIndex = m_graphics_queue_index;
    image_memory_barrier.dstQueueFamilyIndex = m_present_queue_index;
    image_memory_barrier.image = m_swapchain_images[m_current_image];
    image_memory_barrier.subresourceRange = ImageSubresourceRange;

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
      Log::error("Failed to record command buffer.");
      throw std::runtime_error("Failed to record command buffer.");
    }

    const VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &sync.render_semaphore;
    submit_info.pWaitDstStageMask = &wait_stage;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &sync.present_semaphore;

    if (vkQueueSubmit(m_graphics_queue, 1, &submit_info, sync.render_fence) != VK_SUCCESS) {
      Log::error("Failed to submit draw command buffer.");
      throw std::runtime_error("Failed to submit draw command buffer.");
    }

    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &sync.present_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &m_swapchain;
    present_info.pImageIndices = &m_current_image;

    const VkResult result = vkQueuePresentKHR(m_present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
      recreate_swapchain();
    } else if (result != VK_SUCCESS) {
      Log::error("Failed to present swap chain image.");
      throw std::runtime_error("Failed to present swap chain image.");
    }

    m_current_frame = (m_current_frame + 1) % FramesInFlight;
  }

  MemoryCommandBuffer BasicRenderer::current_memory_command_buffer()
  {
    return { m_memops_command_buffers[m_current_memops] };
  }

  void BasicRenderer::defer_release_staging_buffer(StagingBufferReference buffer)
  {
    m_staging_buffers[m_current_memops].push_back(buffer);
  }

  Descriptor BasicRenderer::allocate_descriptor_for_layout(const DescriptorLayout* layout) const
  {
    VkDescriptorPool descriptor_pool = m_descriptor_pools[m_current_frame];

    VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {};
    descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_set_allocate_info.descriptorPool = descriptor_pool;
    descriptor_set_allocate_info.descriptorSetCount = 1;
    descriptor_set_allocate_info.pSetLayouts = &layout->m_layout;

    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

    if (vkAllocateDescriptorSets(m_device, &descriptor_set_allocate_info, &descriptor_set) != VK_SUCCESS) {
      Log::error("Failed to submit draw command buffer.");
      throw std::runtime_error("Failed to allocate descriptor set.");
    }

    return { m_device, descriptor_set };
  }

  RenderTarget BasicRenderer::current_render_target() const
  {
    return { m_swapchain_image_views[m_current_image], m_extent };
  }

  void BasicRenderer::recreate_swapchain()
  {
    Vec2I size = {};
    SDL_Vulkan_GetDrawableSize(m_window, &size.w, &size.h);
    recreate_swapchain(size);
  }

  void BasicRenderer::recreate_swapchain(Vec2I size)
  {
    vkDeviceWaitIdle(m_device);
    construct_swapchain(size);
  }

  void BasicRenderer::wait_idle() const
  {
    vkDeviceWaitIdle(m_device);
  }

  void BasicRenderer::construct_device()
  {
    // instance

    unsigned int extension_count = 0;

    if (SDL_Vulkan_GetInstanceExtensions(m_window, &extension_count, nullptr) == SDL_FALSE) {
      throw std::runtime_error("Failed to get extensions.");
    }

    std::vector<const char*> extension_names(extension_count, nullptr);

    if (SDL_Vulkan_GetInstanceExtensions(m_window, &extension_count, extension_names.data()) == SDL_FALSE) {
      throw std::runtime_error("Failed to get extensions.");
    }

    vkb::InstanceBuilder instance_builder;

    instance_builder.set_engine_name("Gamedev Framework 2")
        .require_api_version(1, 3, 0)
        .enable_validation_layers()
        .enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
        .set_debug_callback(&debug_callback)
        // .add_validation_feature_enable(VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT)
        ;

    // for (auto extension : extension_names) {
    //   Log::info("Required extension: {}", extension);
    //   instance_builder.enable_extension(extension);
    // }

    auto maybe_instance = instance_builder.build();

    if (!maybe_instance) {
      Log::error("Failed to create instance ({}).", maybe_instance.error().message());
      throw std::runtime_error("Failed to create instance.");
    }

    const vkb::Instance instance = maybe_instance.value();

    m_instance = instance.instance;
    m_messenger = instance.debug_messenger;

    // load vulkan functions

    volkLoadInstance(m_instance);

    // surface

    if (SDL_Vulkan_CreateSurface(m_window, m_instance, &m_surface) == SDL_FALSE) {
      Log::error("Failed to create window surface.");
      throw std::runtime_error("failed to create window surface.");
    }

    // physical device

#ifdef GF_USE_LLVMPIPE
    vkb::PhysicalDeviceSelector physical_device_selector(instance, m_surface);

    physical_device_selector.prefer_gpu_device_type(vkb::PreferredDeviceType::cpu)
        .allow_any_gpu_device_type(false);
#else
    const vkb::PhysicalDeviceSelector physical_device_selector(instance, m_surface);
#endif

    // VkPhysicalDeviceVulkan13Features features_13 = {};
    // features_13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    // features_13.dynamicRendering = VK_TRUE;
    //
    // physical_device_selector.set_required_features_13(features_13);

    auto maybe_physical_device = physical_device_selector.select();

    if (!maybe_physical_device) {
      Log::error("Failed to select physical device ({}).", maybe_physical_device.error().message());
      throw std::runtime_error("Failed to select physical device.");
    }

    const vkb::PhysicalDevice physical_device = maybe_physical_device.value();
    m_physical_device = physical_device.physical_device;

    Log::debug("Running on: {}", physical_device.properties.deviceName);

    // device

    VkPhysicalDeviceDynamicRenderingFeatures dynamic_rendering_features = {};
    dynamic_rendering_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
    dynamic_rendering_features.dynamicRendering = VK_TRUE;

    vkb::DeviceBuilder device_builder(physical_device);

    device_builder.add_pNext(&dynamic_rendering_features);

    auto maybe_device = device_builder.build();

    if (!maybe_device) {
      Log::error("Failed to create device ({}).", maybe_device.error().message());
      throw std::runtime_error("Failed to create device.");
    }

    const vkb::Device device = maybe_device.value();

    m_device = device.device;

    auto maybe_graphics_queue_index = device.get_queue_index(vkb::QueueType::graphics);

    if (!maybe_graphics_queue_index) {
      Log::error("Failed to get a graphics queue ({}).", maybe_graphics_queue_index.error().message());
      throw std::runtime_error("Failed to get a graphics queue.");
    }

    // load vulkan device functions

    volkLoadDevice(m_device);

    // queues

    m_graphics_queue_index = maybe_graphics_queue_index.value();
    vkGetDeviceQueue(m_device, m_graphics_queue_index, 0, &m_graphics_queue);

    auto maybe_present_queue_index = device.get_queue_index(vkb::QueueType::present);

    if (!maybe_present_queue_index) {
      Log::error("Failed to get a present queue ({}).", maybe_present_queue_index.error().message());
      throw std::runtime_error("Failed to get a present queue.");
    }

    m_present_queue_index = maybe_present_queue_index.value();
    vkGetDeviceQueue(m_device, m_present_queue_index, 0, &m_present_queue);

    Log::debug("Queue indices: graphics {}, present {}", m_graphics_queue_index, m_present_queue_index);
  }

  void BasicRenderer::destroy_device()
  {
    if (m_device != VK_NULL_HANDLE) {
      vkDestroyDevice(m_device, nullptr);
    }

    if (m_surface != VK_NULL_HANDLE) {
      vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    if (m_messenger != VK_NULL_HANDLE) {
      vkDestroyDebugUtilsMessengerEXT(m_instance, m_messenger, nullptr);
    }

    if (m_instance != VK_NULL_HANDLE) {
      vkDestroyInstance(m_instance, nullptr);
    }
  }

  void BasicRenderer::construct_allocator()
  {
    VmaVulkanFunctions functions = {};
    functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocator_info = {};
    allocator_info.physicalDevice = m_physical_device;
    allocator_info.device = m_device;
    allocator_info.instance = m_instance;
    allocator_info.pVulkanFunctions = &functions;
    allocator_info.vulkanApiVersion = VK_API_VERSION_1_3;

    if (vmaCreateAllocator(&allocator_info, &m_allocator) != VK_SUCCESS) {
      Log::error("Failed to create an allocator.");
      throw std::runtime_error("Failed to create an allocator.");
    }
  }

  void BasicRenderer::destroy_allocator()
  {
    if (m_allocator != nullptr) {
      vmaDestroyAllocator(m_allocator);
    }
  }

  void BasicRenderer::construct_swapchain(Vec2I size)
  {
    if (size.w == 0 || size.h == 0) {
      // handle minimization
      return;
    }

    vkb::SwapchainBuilder swapchain_builder(m_physical_device, m_device, m_surface, m_graphics_queue_index, m_present_queue_index);

    swapchain_builder.set_old_swapchain(m_swapchain)
        .set_desired_extent(static_cast<uint32_t>(size.w), static_cast<uint32_t>(size.h))
        .use_default_format_selection()
        .use_default_present_mode_selection()
        .use_default_image_usage_flags()
        .set_desired_min_image_count(vkb::SwapchainBuilder::TRIPLE_BUFFERING);

    auto maybe_swapchain = swapchain_builder.build();

    if (!maybe_swapchain) {
      Log::error("Failed to create a swapchain ({}).", maybe_swapchain.error().message());
      throw std::runtime_error("Failed to create a swapchain.");
    }

    vkb::Swapchain swapchain = maybe_swapchain.value();

    if (m_swapchain != VK_NULL_HANDLE) {
      // in case of swapchain recreation
      destroy_swapchain();
    }

    m_swapchain = swapchain.swapchain;
    m_image_count = swapchain.image_count;
    m_format = swapchain.image_format;
    m_extent = swapchain.extent;

    VkSurfaceCapabilitiesKHR capabilities = {};

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, m_surface, &capabilities) != VK_SUCCESS) {
      Log::error("Failed to get surface capabilities.");
      throw std::runtime_error("Failed to get surface capabilities.");
    }

    Log::debug("Swapchain image count: {} (min requested: {}, min: {}, max: {})", m_image_count, swapchain.requested_min_image_count, capabilities.minImageCount, capabilities.maxImageCount);
    Log::debug("Swapchain extent: {}x{}", m_extent.width, m_extent.height);

    // images

    auto maybe_images = swapchain.get_images();

    if (!maybe_images) {
      Log::error("Failed to get images ({}).", maybe_images.error().message());
      throw std::runtime_error("Failed to get images.");
    }

    m_swapchain_images = maybe_images.value();

    // image_views

    auto maybe_image_views = swapchain.get_image_views();

    if (!maybe_image_views) {
      Log::error("Failed to get image views ({}).", maybe_image_views.error().message());
      throw std::runtime_error("Failed to get image views.");
    }

    m_swapchain_image_views = maybe_image_views.value();

    // Log::info("New swapchain extent: ({}, {}).", m_extent.width, m_extent.height);
  }

  void BasicRenderer::destroy_swapchain()
  {
    for (VkImageView image_view : m_swapchain_image_views) {
      vkDestroyImageView(m_device, image_view, nullptr);
    }

    if (m_swapchain != VK_NULL_HANDLE) {
      vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    }
  }

  void BasicRenderer::construct_commands()
  {
    // render command buffers

    VkCommandPoolCreateInfo command_pool_info = {};
    command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_info.queueFamilyIndex = m_graphics_queue_index;

    if (vkCreateCommandPool(m_device, &command_pool_info, nullptr, &m_command_pool) != VK_SUCCESS) {
      Log::error("Failed to create command pool.");
      throw std::runtime_error("Failed to create command pool.");
    }

    m_command_buffers.resize(FramesInFlight);

    VkCommandBufferAllocateInfo allocate_info = {};
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = m_command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_device, &allocate_info, m_command_buffers.data()) != VK_SUCCESS) {
      Log::error("Failed to allocate command buffers.");
      throw std::runtime_error("Failed to allocate command buffers.");
    }

    // memory command buffers

    if (vkCreateCommandPool(m_device, &command_pool_info, nullptr, &m_memops_command_pool) != VK_SUCCESS) {
      Log::error("Failed to create command pool.");
      throw std::runtime_error("Failed to create command pool.");
    }

    m_memops_command_buffers.resize(FramesInFlight);

    allocate_info.commandPool = m_memops_command_pool;
    allocate_info.commandBufferCount = static_cast<uint32_t>(m_memops_command_buffers.size());
    ;

    if (vkAllocateCommandBuffers(m_device, &allocate_info, m_memops_command_buffers.data()) != VK_SUCCESS) {
      Log::error("Failed to allocate command buffer.");
      throw std::runtime_error("Failed to allocate command buffer.");
    }

    // begin the memory command buffer

    assert(m_current_memops == 0);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(m_memops_command_buffers[m_current_memops], &begin_info) != VK_SUCCESS) {
      Log::error("Failed to begin recording command buffer.");
      throw std::runtime_error("Failed to begin recording command buffer.");
    }

    // staging buffers

    m_staging_buffers.resize(FramesInFlight);
  }

  void BasicRenderer::destroy_commands()
  {
    if (m_memops_command_pool != VK_NULL_HANDLE) {
      vkDestroyCommandPool(m_device, m_memops_command_pool, nullptr);
    }

    if (m_command_pool != VK_NULL_HANDLE) {
      vkDestroyCommandPool(m_device, m_command_pool, nullptr);
    }
  }

  void BasicRenderer::construct_synchronization()
  {
    m_render_synchronization.resize(FramesInFlight);

    VkSemaphoreCreateInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (RenderSynchronizationObjects& objects : m_render_synchronization) {
      if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &objects.present_semaphore) != VK_SUCCESS) {
        Log::error("Failed to create present semaphore.");
        throw std::runtime_error("Failed to create present semaphore.");
      }

      if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &objects.render_semaphore) != VK_SUCCESS) {
        Log::error("Failed to create render semaphore.");
        throw std::runtime_error("Failed to create render semaphore.");
      }

      fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      if (vkCreateFence(m_device, &fence_info, nullptr, &objects.render_fence) != VK_SUCCESS) {
        Log::error("Failed to create render fence.");
        throw std::runtime_error("Failed to create render fence.");
      }

      fence_info.flags = 0;

      if (vkCreateFence(m_device, &fence_info, nullptr, &objects.memops_fence) != VK_SUCCESS) {
        Log::error("Failed to create memops fence.");
        throw std::runtime_error("Failed to create memops fence.");
      }
    }
  }

  void BasicRenderer::destroy_synchronization()
  {
    for (const RenderSynchronizationObjects& objects : m_render_synchronization) {
      vkDestroyFence(m_device, objects.memops_fence, nullptr);
      vkDestroyFence(m_device, objects.render_fence, nullptr);
      vkDestroySemaphore(m_device, objects.render_semaphore, nullptr);
      vkDestroySemaphore(m_device, objects.present_semaphore, nullptr);
    }
  }

  void BasicRenderer::construct_descriptors()
  {
    m_descriptor_pools.resize(FramesInFlight);

    VkDescriptorPoolSize pool_sizes[] = {
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 64 },
      {         VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 64 },
    };

    VkDescriptorPoolCreateInfo descriptor_pool_info = {};
    descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
    descriptor_pool_info.pPoolSizes = std::begin(pool_sizes);
    descriptor_pool_info.maxSets = 64;

    for (std::size_t i = 0; i < FramesInFlight; ++i) {
      if (vkCreateDescriptorPool(m_device, &descriptor_pool_info, nullptr, &m_descriptor_pools[i]) != VK_SUCCESS) {
        Log::error("Failed to create descriptor pool.");
        throw std::runtime_error("Failed to create descriptor pool.");
      }
    }
  }

  void BasicRenderer::destroy_descriptors()
  {
    for (VkDescriptorPool descriptor_pool : m_descriptor_pools) {
      vkDestroyDescriptorPool(m_device, descriptor_pool, nullptr);
    }
  }

  namespace {

    const uint32_t simple_vert_shader_code[] = {
#include "simple.vert.h"
    };

    const uint32_t simple_frag_shader_code[] = {
#include "simple.frag.h"
    };

    const uint32_t default_vert_shader_code[] = {
#include "default.vert.h"
    };

    const uint32_t default_frag_shader_code[] = {
#include "default.frag.h"
    };

    const uint32_t text_frag_shader_code[] = {
#include "text.frag.h"
    };

    const uint32_t fullscreen_vert_shader_code[] = {
#include "fullscreen.vert.h"
    };

    const uint32_t fullscreen_frag_shader_code[] = {
#include "fullscreen.frag.h"
    };
  }

  Renderer::Renderer(Window* window)
  : BasicRenderer(window)
  {
    build_default_pipelines();
  }

  void Renderer::update_surface_size(Vec2I size)
  {
    recreate_swapchain(size);
  }

  void Renderer::build_default_pipelines()
  {
    // view matrix descriptor

    DescriptorBinding camera_binding[] = {
      { 0, DescriptorType::Buffer, ShaderStage::Vertex }
    };

    m_camera_descriptor = DescriptorLayout(camera_binding, this);

    // sampler descriptor

    DescriptorBinding sampler_binding[] = {
      { 0, DescriptorType::Sampler, ShaderStage::Fragment }
    };

    m_sampler_descriptor = DescriptorLayout(sampler_binding, this);

    // simple pipeline layout

    RenderPipelineLayoutBuilder simple_pipeline_layout_builder;

    simple_pipeline_layout_builder.set_push_constant_parameters(ShaderStage::Vertex, sizeof(ShaderDataType<Mat3F>))
        .add_descriptor_layout(&m_camera_descriptor);

    m_simple_pipeline_layout = simple_pipeline_layout_builder.build(this);
    m_simple_pipeline_layout.set_debug_name("Simple Pipeline Layout");

    // default pipeline layout

    RenderPipelineLayoutBuilder default_pipeline_layout_builder;

    default_pipeline_layout_builder.set_push_constant_parameters(ShaderStage::Vertex, sizeof(ShaderDataType<Mat3F>))
        .add_descriptor_layout(&m_camera_descriptor)
        .add_descriptor_layout(&m_sampler_descriptor);

    m_default_pipeline_layout = default_pipeline_layout_builder.build(this);
    m_default_pipeline_layout.set_debug_name("Default Pipeline Layout");

    // fullscreen pipeline layout

    RenderPipelineLayoutBuilder fullscreen_pipeline_layout_builder;
    fullscreen_pipeline_layout_builder.add_descriptor_layout(&m_sampler_descriptor);
    m_fullscreen_pipeline_layout = fullscreen_pipeline_layout_builder.build(this);
    m_fullscreen_pipeline_layout.set_debug_name("Fullscreen Pipeline Layout");

    // simple pipeline

    Shader simple_vertex_shader(gf::span(simple_vert_shader_code), { ShaderStage::Vertex, this });
    Shader simple_fragment_shader(gf::span(simple_frag_shader_code), { ShaderStage::Fragment, this });

    RenderPipelineBuilder simple_pipeline_builder;

    simple_pipeline_builder.set_vertex_input(SimpleVertex::compute_input())
        .add_shader(&simple_vertex_shader)
        .add_shader(&simple_fragment_shader)
        .set_pipeline_layout(&m_simple_pipeline_layout);

    m_simple_pipeline = simple_pipeline_builder.build(this);
    m_simple_pipeline.set_debug_name("Simple Pipeline");

    // default pipeline

    Shader default_vertex_shader(gf::span(default_vert_shader_code), { ShaderStage::Vertex, this });
    Shader default_fragment_shader(gf::span(default_frag_shader_code), { ShaderStage::Fragment, this });

    RenderPipelineBuilder default_pipeline_builder;

    default_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(&default_vertex_shader)
        .add_shader(&default_fragment_shader)
        .set_pipeline_layout(&m_default_pipeline_layout)
        ;

    m_default_pipeline = default_pipeline_builder.build(this);
    m_default_pipeline.set_debug_name("Default Pipeline");

    // text pipeline

    Shader text_fragment_shader(gf::span(text_frag_shader_code), { ShaderStage::Fragment, this });

    RenderPipelineBuilder text_pipeline_builder;

    text_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(&default_vertex_shader)
        .add_shader(&text_fragment_shader)
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_text_pipeline = text_pipeline_builder.build(this);
    m_text_pipeline.set_debug_name("Text Pipeline");

    // fullscreen pipeline

    Shader fullscreen_vertex_shader(gf::span(fullscreen_vert_shader_code), { ShaderStage::Vertex, this });
    Shader fullscreen_fragment_shader(gf::span(fullscreen_frag_shader_code), { ShaderStage::Fragment, this });

    RenderPipelineBuilder fullscreen_pipeline_builder;

    fullscreen_pipeline_builder.add_shader(&fullscreen_vertex_shader)
        .add_shader(&fullscreen_fragment_shader)
        .set_pipeline_layout(&m_fullscreen_pipeline_layout);

    m_fullscreen_pipeline = fullscreen_pipeline_builder.build(this);
    m_fullscreen_pipeline.set_debug_name("Fullscreen Pipeline");
  }

}
