// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/RenderManager.h>

#include <thread>
#include <utility>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>
#include <gf2/core/Mat3.h>

#include <gf2/graphics/Vertex.h>
#include "SDL3/SDL_gpu.h"

namespace gf {

  RenderManager::RenderManager(Window* window)
  : m_window(window->m_window)
  , m_thread_id(std::this_thread::get_id())
  {
    Vec2I size = {};
    SDL_GetWindowSizeInPixels(m_window, &size.w, &size.h);

    m_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");

    if (!m_device) {
      Log::fatal("Could not create device: {}", SDL_GetError());
    }

    if (!SDL_ClaimWindowForGPUDevice(m_device, m_window)) {
      Log::fatal("Could not claim window: {}", SDL_GetError());
    }

    MemOps& memops = m_memops[m_current_memops];
    memops.command_buffer = SDL_AcquireGPUCommandBuffer(m_device);

    if (memops.command_buffer == nullptr) {
      Log::fatal("Could not acquire command buffer: {}", SDL_GetError());
    }

    memops.copy_pass = SDL_BeginGPUCopyPass(memops.command_buffer);

    if (memops.copy_pass == nullptr) {
      Log::fatal("Could not begin copy pass: {}", SDL_GetError());
    }

  }

  RenderManager::~RenderManager()
  {
    assert(m_current_frame == m_current_memops);
    RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];

    if (sync.render_fence != nullptr) {
      SDL_GPUFence* render_fence = sync.render_fence;
      SDL_WaitForGPUFences(m_device, true, &render_fence, 1);
    }

    MemOps& memops = m_memops[m_current_memops];
    SDL_EndGPUCopyPass(memops.copy_pass);

    SDL_CancelGPUCommandBuffer(memops.command_buffer);

    SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
  }

  Vec2I RenderManager::surface_size() const
  {
    return m_swapchain_texture_size;
  }

  GpuTextureFormat RenderManager::surface_format()
  {
    return static_cast<GpuTextureFormat>(SDL_GetGPUSwapchainTextureFormat(m_device, m_window));
  }

  GpuCommandBuffer RenderManager::begin_command_buffer()
  {
    assert(m_current_frame == m_current_memops);
    RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];

    // synchronize and acquire the next image

    if (sync.render_fence != nullptr) {
      SDL_GPUFence* render_fence = sync.render_fence;
      SDL_WaitForGPUFences(m_device, true, &render_fence, 1);
    }

    // submit memory command buffer

    MemOps& memops = m_memops[m_current_memops];
    SDL_EndGPUCopyPass(memops.copy_pass);
    sync.memops_fence = { m_device, SDL_SubmitGPUCommandBufferAndAcquireFence(memops.command_buffer) };

    // begin next memory command buffer

    m_current_memops = (m_current_memops + 1) % FramesInFlight;

    MemOps& next_memops = m_memops[m_current_memops];
    next_memops.command_buffer = SDL_AcquireGPUCommandBuffer(m_device);
    next_memops.copy_pass = SDL_BeginGPUCopyPass(next_memops.command_buffer);

    // begin render command buffer

    SDL_GPUCommandBuffer* command_buffer = m_command_buffers[m_current_frame] = SDL_AcquireGPUCommandBuffer(m_device);

    Uint32 swapchain_texture_width = 0;
    Uint32 swapchain_texture_height = 0;

    if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, m_window, &m_swapchain_texture, &swapchain_texture_width, &swapchain_texture_height)) {
      Log::fatal("Could not acquire swapchain texture: {}", SDL_GetError());
    }

    m_swapchain_texture_size = vec(swapchain_texture_width, swapchain_texture_height);

    return command_buffer;
  }

  void RenderManager::end_command_buffer([[maybe_unused]] GpuCommandBuffer buffer)
  {
    RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];

    // wait for memops end

    SDL_GPUFence* memops_fence = sync.memops_fence;
    SDL_WaitForGPUFences(m_device, true, &memops_fence, 1);

    // delete staging buffers

    m_transfer_buffers[m_current_frame].clear();

    // submit render command buffer

    sync.render_fence = { m_device, SDL_SubmitGPUCommandBufferAndAcquireFence(m_command_buffers[m_current_frame]) };

    m_current_frame = (m_current_frame + 1) % FramesInFlight;
  }

  GpuCopyPass RenderManager::current_copy_pass()
  {
    if (m_thread_id == std::this_thread::get_id()) {
      return m_memops[m_current_memops].copy_pass;
    }

    assert(m_async_loading);
    return m_async_memops.copy_pass;
  }

  void RenderManager::defer_release_transfer_buffer(GpuTransferBuffer buffer)
  {
    if (m_thread_id == std::this_thread::get_id()) {
      m_transfer_buffers[m_current_memops].push_back(std::move(buffer));
      return;
    }

    assert(m_async_loading);
    m_async_staging_buffers.push_back(std::move(buffer));
  }

  void RenderManager::prepare_asynchronous_load()
  {
    assert(!m_async_loading);
    m_async_loading = true;
  }

  void RenderManager::begin_asynchronous_load()
  {
    assert(m_async_loading);

    m_async_memops.command_buffer = SDL_AcquireGPUCommandBuffer(m_device);
    m_async_memops.copy_pass = SDL_BeginGPUCopyPass(m_async_memops.command_buffer);
  }

  void RenderManager::end_asynchronous_load()
  {
    assert(m_async_loading);

    SDL_EndGPUCopyPass(m_async_memops.copy_pass);
    SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(m_async_memops.command_buffer);

    SDL_WaitForGPUFences(m_device, true, &fence, 1);

    m_async_staging_buffers.clear();
  }

  void RenderManager::finish_asynchronous_load()
  {
    assert(m_async_loading);
    m_async_loading = false;
  }

  RenderTarget RenderManager::current_render_target() const
  {
    return { m_swapchain_texture };
  }

  void RenderManager::wait_idle()
  {
    SDL_WaitForGPUIdle(m_device);
  }

  GpuDevice* RenderManager::device()
  {
    return &m_device;
  }

}
