// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHADER_STAGE_H
#define GF_SHADER_STAGE_H

#include <type_traits>

#include <vulkan/vulkan.h>

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class ShaderStage : std::underlying_type_t<VkShaderStageFlagBits> {
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
  };

}

#endif // GF_SHADER_STAGE_H
