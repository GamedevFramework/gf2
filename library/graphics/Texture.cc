// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Texture.h>

#include <cassert>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  using namespace operators;

  Texture::Texture(const std::filesystem::path& filename, RenderManager* render_manager)
  : Texture(Image(filename), render_manager)
  {
  }

  Texture::Texture(const Image& image, RenderManager* render_manager)
  : Texture(image.size(), TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Color8S, render_manager)
  {
    update(image.raw_size(), image.raw_data(), render_manager);
  }

  Texture::Texture(const Bitmap& bitmap, RenderManager* render_manager)
  : Texture(bitmap.size(), TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Gray8U, render_manager)
  {
    update(bitmap.raw_size(), bitmap.raw_data(), render_manager);
  }

  Texture::Texture(Vec2I size, RenderManager* render_manager)
  : Texture(size, TextureUsage::ColorTarget | TextureUsage::Sampled, Format::RenderManager, render_manager)
  {
  }

  Texture::Texture(Vec2I size, Flags<TextureUsage> usage, Format format, RenderManager* render_manager)
  : m_image_size(size)
  , m_usage(usage)
  , m_format(format)
  {
    // image view and sampler

    create_image_view_and_sampler(format, render_manager);
  }

  void Texture::set_debug_name(const std::string& name) const
  {
  }

  void Texture::update(const Image& image, RenderManager* render_manager)
  {
    assert(image.size() == m_image_size);
    assert(m_format == Format::Color8S);
    assert(m_usage.test(TextureUsage::TransferDestination));

    update(image.raw_size(), image.raw_data(), render_manager);
  }

  void Texture::update(const Bitmap& bitmap, RenderManager* render_manager)
  {
    assert(bitmap.size() == m_image_size);
    assert(m_format == Format::Gray8U);
    assert(m_usage.test(TextureUsage::TransferDestination));

    update(bitmap.raw_size(), bitmap.raw_data(), render_manager);
  }

  void Texture::update(std::size_t size, const uint8_t* data, RenderManager* render_manager)
  {
    auto staging = create_staging_buffer(size, data);
    compute_memory_operations(staging, render_manager);
  }

  RenderTarget Texture::as_render_target() const
  {
    return { };
  }

  Texture::operator TextureReference() const
  {
    return { };
  }

  StagingBufferReference Texture::create_staging_buffer(std::size_t raw_size, const void* raw_data)
  {
    return { };
  }

  void Texture::create_image_view_and_sampler(Format format, RenderManager* render_manager)
  {
  }

  void Texture::compute_memory_operations(StagingBufferReference staging, RenderManager* render_manager)
  {
    auto command_buffer = render_manager->current_memory_command_buffer();

    render_manager->defer_release_staging_buffer(staging);
  }

}
