// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuGraphicsPipeline.h>

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

  void GpuGraphicsPipeline::set_debug_name(const std::string& name) const
  {
  }

  /*
   * RenderPipelineBuilder
   */

  GpuGraphicsPipeline GpuGraphicsPipelineBuilder::build(RenderManager* render_manager)
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
