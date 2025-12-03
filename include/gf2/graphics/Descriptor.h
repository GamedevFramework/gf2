// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_DESCRIPTOR_H
#define GF_DESCRIPTOR_H

#include <type_traits>

#include <gf2/core/Span.h>

#include "GraphicsApi.h"
#include "Shader.h"

namespace gf {
  class Buffer;
  class Texture;
  class RenderManager;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class DescriptorType {
    Sampler,
    Buffer,
  };

  struct GF_GRAPHICS_API DescriptorBinding {
    uint32_t binding = 0;
    DescriptorType type = DescriptorType::Sampler;
    GpuShaderStage stage = GpuShaderStage::Vertex;
  };

  class GF_GRAPHICS_API DescriptorLayout {
  public:
    DescriptorLayout() = default;
    DescriptorLayout(Span<const DescriptorBinding> bindings, RenderManager* render_manager);

  private:
    friend class RenderManager;
    friend class RenderPipelineLayoutBuilder;
  };

  class GF_GRAPHICS_API Descriptor {
  public:

    void write(uint32_t binding, const Texture* texture);
    void write(uint32_t binding, const Buffer* buffer);

  private:
    friend class RenderManager;
    friend class RenderCommandBuffer;

  };

}

#endif // GF_DESCRIPTOR_H
