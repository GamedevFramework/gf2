// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/RenderAsync.h>
// clang-format on

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {
  RenderAsync::RenderAsync(RenderManager* render_manager)
  : m_render_manager(render_manager)
  {
  }

  void RenderAsync::run_async(std::function<void()> function)
  {
    m_clock.restart();
    m_render_manager->prepare_asynchronous_load();

    m_result = std::async(std::launch::async, [&,function=std::move(function)]() {
      m_render_manager->begin_asynchronous_load();
      function();
      m_render_manager->end_asynchronous_load();
    });
  }

  bool RenderAsync::finished()
  {
    if (m_result.valid() && m_result.wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {
      m_result.get();
      m_render_manager->finish_asynchronous_load();
      auto time = m_clock.restart();
      Log::info("Asynchronous loading finished in {}ms", time.as_milliseconds());
      return true;
    }

    return false;
  }

}
