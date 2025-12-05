// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/RenderPipeline.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <utility>
#include <vector>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  /*
   * RenderPipeline
   */

  void RenderPipeline::set_debug_name(const std::string& name) const
  {
  }

  /*
   * RenderPipelineBuilder
   */

  RenderPipeline RenderPipelineBuilder::build(RenderManager* render_manager)
  {
    // shaders

    // vertex input

    // input assembly

    // viewport

    // rasterizer

    // multisampling

    // color blending

    // pipeline rendering

    // pipeline

    // destroy shader modules

    return { };
  }

}
