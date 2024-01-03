// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RENDER_ASYNC_H
#define GF_RENDER_ASYNC_H

#include <functional>
#include <future>

#include <gf2/core/Clock.h>

namespace gf {

  class RenderManager;

  class RenderAsync {
  public:
    RenderAsync(RenderManager* render_manager);

    void run_async(std::function<void()> function);
    bool finished();

  private:
    RenderManager* m_render_manager;
    Clock m_clock;
    std::future<void> m_result;
  };

}

#endif // GF_RENDER_ASYNC_H
