#ifndef GF_RENDER_TARGET_H
#define GF_RENDER_TARGET_H

#include <vulkan/vulkan.h>

#include "GraphicsApi.h"
#include "Vec2.h"

namespace gf {

  class GF_GRAPHICS_API RenderTarget {
  public:

    Vec2<uint32_t> extent() const
    {
      return { m_extent.width, m_extent.height };
    }

  private:
    friend class CommandBuffer;
    friend class Renderer;

    RenderTarget(VkImageView image_view, VkExtent2D extent)
    : m_image_view(image_view)
    , m_extent(extent)
    {
    }

    VkImageView m_image_view = VK_NULL_HANDLE;
    VkExtent2D m_extent = { 0, 0 };
  };

}

#endif // GF_RENDER_TARGET_H
