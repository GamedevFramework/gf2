// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FORMAT_H
#define GF_FORMAT_H

#include <limits>
#include <type_traits>

#include <vulkan/vulkan.h>

namespace gf {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class Format : std::underlying_type_t<VkFormat> {
    Undefined = VK_FORMAT_UNDEFINED,
    Color8S = VK_FORMAT_R8G8B8A8_SRGB,
    Color8U = VK_FORMAT_R8G8B8A8_UNORM,
    Color32F = VK_FORMAT_R32G32B32A32_SFLOAT,
    Gray8U = VK_FORMAT_R8_UNORM,
    Vec2F = VK_FORMAT_R32G32_SFLOAT,
    Renderer = std::numeric_limits<std::underlying_type_t<VkFormat>>::max(),
  };

}

#endif // GF_FORMAT_H
