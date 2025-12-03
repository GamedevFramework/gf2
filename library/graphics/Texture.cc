// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Texture.h>

#include <cassert>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>

namespace gf {

  using namespace operators;

  Texture::Texture(const std::filesystem::path& filename, RenderManager* render_manager)
  : Texture(Image(filename), render_manager)
  {
  }

  Texture::Texture(const Image& image, RenderManager* render_manager)
  : Texture(image.size(), TextureUsage::Sampler, Format::R8G8B8A8_SNorm, render_manager)
  {
    update(image.raw_size(), image.raw_data(), render_manager);
  }

  Texture::Texture(const Bitmap& bitmap, RenderManager* render_manager)
  : Texture(bitmap.size(), TextureUsage::Sampler, Format::R8_UNorm, render_manager)
  {
    update(bitmap.raw_size(), bitmap.raw_data(), render_manager);
  }

  Texture::Texture(Vec2I size, RenderManager* render_manager)
  : Texture(size, TextureUsage::ColorTarget | TextureUsage::Sampler, Format::RenderManager, render_manager)
  {
  }

  Texture::Texture(Vec2I size, Flags<TextureUsage> usage, Format format, RenderManager* render_manager)
  : m_image_size(size)
  , m_usage(usage)
  , m_format(format)
  {
    GpuDevice& device = render_manager->device();

    SDL_GPUTextureCreateInfo texture_info = { };
    texture_info.type = SDL_GPU_TEXTURETYPE_2D;
    texture_info.format = static_cast<SDL_GPUTextureFormat>(m_format);
    texture_info.usage = static_cast<SDL_GPUTextureUsageFlags>(m_usage);
    texture_info.width = static_cast<uint32_t>(size.w);
    texture_info.height = static_cast<uint32_t>(size.h);

    m_texture_handle = { device, SDL_CreateGPUTexture(device, &texture_info) };

    if (m_usage.test(gf::TextureUsage::Sampler)) {
      SDL_GPUSamplerCreateInfo sampler_info = { };
      sampler_info.min_filter = SDL_GPU_FILTER_LINEAR;
      sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR;
      sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
      sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
      sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
      sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
      sampler_info.mip_lod_bias = 0.0f;
      sampler_info.max_anisotropy = 0.0f;
      sampler_info.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
      sampler_info.min_lod = 0.0f;
      sampler_info.max_lod = 0.0f;
      sampler_info.enable_anisotropy = false;
      sampler_info.enable_compare = false;

      m_sampler_handle = { device, SDL_CreateGPUSampler(device, &sampler_info) };
    }
  }

  void Texture::set_debug_name(const std::string& name)
  {
    SDL_SetGPUTextureName(m_texture_handle.device(), m_texture_handle, name.c_str());
  }

  void Texture::update(const Image& image, RenderManager* render_manager)
  {
    assert(image.size() == m_image_size);
    assert(m_format == Format::R8G8B8A8_SNorm);

    update(image.raw_size(), image.raw_data(), render_manager);
  }

  void Texture::update(const Bitmap& bitmap, RenderManager* render_manager)
  {
    assert(bitmap.size() == m_image_size);
    assert(m_format == Format::R8_UNorm);

    update(bitmap.raw_size(), bitmap.raw_data(), render_manager);
  }

  void Texture::update(std::size_t size, const uint8_t* data, RenderManager* render_manager)
  {
    TransferBuffer buffer(size * sizeof(uint8_t), render_manager);
    buffer.update(data, size);

    CopyPass copy_pass = render_manager->current_copy_pass();
    copy_pass.copy_buffer_to_texture(&buffer, this, m_image_size);
    render_manager->defer_release_transfer_buffer(std::move(buffer));
  }

  RenderTarget Texture::as_render_target()
  {
    return { m_texture_handle };
  }

  Texture::operator TextureReference()
  {
    return {  };
  }

}
