// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHADER_H
#define GF_SHADER_H

#include <cstdint>

#include <filesystem>

#include <vulkan/vulkan.h>

#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "ShaderStage.h"

namespace gf {
  class Renderer;

  struct GF_GRAPHICS_API ShaderContext {
    ShaderStage stage;
    Renderer* renderer;
  };

  class GF_GRAPHICS_API Shader {
  public:
    using Context = ShaderContext;

    Shader() = default;
    Shader(const std::filesystem::path& filename, ShaderContext context);
    Shader(Span<const uint32_t> memory, ShaderContext context);
    Shader(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    ~Shader();

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept;

    ShaderStage stage() const
    {
      return m_stage;
    }

  private:
    friend class RenderPipelineBuilder;

    void load_code(Span<const uint32_t> code, VkDevice device);

    ShaderStage m_stage = ShaderStage::Vertex;
    VkDevice m_device = VK_NULL_HANDLE; // non-owning
    VkShaderModule m_shader_module = VK_NULL_HANDLE;
  };

}

#endif // GF_SHADER_H
