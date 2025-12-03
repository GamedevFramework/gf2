// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Descriptor.h>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/graphics/GpuTexture.h>
#include <gf2/graphics/RenderManager.h>

namespace gf {

  /*
   * DescriptorLayout
   */

  DescriptorLayout::DescriptorLayout(Span<const DescriptorBinding> bindings, RenderManager* render_manager)
  {
    assert(!bindings.empty());
  }

  /*
   * Descriptor
   */

  void Descriptor::write(uint32_t binding, const GpuTexture* texture)
  {
  }

  void Descriptor::write(uint32_t binding, const Buffer* buffer)
  {
  }

}
