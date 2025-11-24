// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/RenderManager.h>

#include <mutex>
#include <thread>
#include <utility>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>
#include <gf2/core/Mat3.h>

#include <gf2/graphics/Vertex.h>

// #define GF_USE_LLVMPIPE

namespace gf {

  constexpr std::size_t FramesInFlight = 3;

  RenderManager::RenderManager(Window* window)
  : m_window(window->m_window)
  , m_thread_id(std::this_thread::get_id())
  {
    Vec2I size = {};
    SDL_GetWindowSizeInPixels(m_window, &size.w, &size.h);
  }

  void RenderManager::update_surface_size(Vec2I size)
  {
    // still needed?
  }

  Vec2I RenderManager::surface_size() const
  {
    return { 0, 0 };
  }

  std::optional<CommandBuffer> RenderManager::begin_command_buffer()
  {
    const RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];
    assert(m_current_frame == m_current_memops);

    // synchronize and acquire the next image


    // submit memory command buffer


    // begin next memory command buffer

    m_current_memops = (m_current_memops + 1) % FramesInFlight;


    // begin render command buffer

    return { };
  }

  void RenderManager::end_command_buffer([[maybe_unused]] CommandBuffer buffer)
  {
    const RenderSynchronizationObjects& sync = m_render_synchronization[m_current_frame];

    // wait for memops end

    // delete staging buffers

    // submit render command buffer


    m_current_frame = (m_current_frame + 1) % FramesInFlight;
  }

  CopyPass RenderManager::current_copy_pass()
  {
    if (m_thread_id == std::this_thread::get_id()) {
      return { };
    }

    assert(m_async_loading);
    return { };
  }

  void RenderManager::defer_release_staging_buffer(StagingBufferReference buffer)
  {
    if (m_thread_id == std::this_thread::get_id()) {
      m_staging_buffers[m_current_memops].push_back(buffer);
      return;
    }

    assert(m_async_loading);
    m_async_staging_buffers.push_back(buffer);
  }

  void RenderManager::prepare_asynchronous_load()
  {
    assert(!m_async_loading);
    m_async_loading = true;
  }

  void RenderManager::begin_asynchronous_load()
  {
    assert(m_async_loading);

  }

  void RenderManager::end_asynchronous_load()
  {
    assert(m_async_loading);


    m_async_staging_buffers.clear();

  }

  void RenderManager::finish_asynchronous_load()
  {
    assert(m_async_loading);
    m_async_loading = false;
  }

  Descriptor RenderManager::allocate_descriptor_for_layout(const DescriptorLayout* layout) const
  {

    return { };
  }

  RenderTarget RenderManager::current_render_target() const
  {
    return { };
  }

  void RenderManager::wait_idle() const
  {
  }

  void RenderManager::finish_staging_buffers()
  {
    for (auto& staging_buffers : m_staging_buffers) {
      for (auto staging_buffer : staging_buffers) {
      }
    }
  }

}
