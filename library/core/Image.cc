// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Image.h>

#include <cstdio>

#include <algorithm>

// clang-format off: main header
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stb_image_write.h>
// clang-format on

#include <gf2/core/Blit.h>
#include <gf2/core/Log.h>
#include <gf2/core/Stream.h>

namespace gf {

  namespace {
    std::size_t compute_image_size(Vec2I size)
    {
      return 4 * static_cast<std::size_t>(size.w) * static_cast<std::size_t>(size.h);
    }

    uint8_t to_byte(float channel)
    {
      return static_cast<uint8_t>(channel * 255);
    }

    float to_float(uint8_t byte)
    {
      return static_cast<float>(byte) / 255.0f;
    }

    int stb_callback_read(void* user, char* data, int size)
    {
      auto* stream = static_cast<InputStream*>(user);
      return static_cast<int>(stream->read(Span<uint8_t>(reinterpret_cast<uint8_t*>(data), size))); // NOLINT
    }

    void stb_callback_skip(void* user, int n)
    {
      auto* stream = static_cast<InputStream*>(user);
      stream->skip(n);
    }

    int stb_callback_eof(void* user)
    {
      auto* stream = static_cast<InputStream*>(user);
      return static_cast<int>(stream->finished());
    }

    struct FileCloseDeleter {
      void operator()(std::FILE* file) const noexcept
      {
        std::fclose(file);
      }
    };

  } // namespace

  Image::Image()
  : m_size(0, 0)
  {
  }

  Image::Image(Vec2I size)
  : m_size(size)
  , m_pixels(compute_image_size(size), 0xFF)
  {
  }

  Image::Image(Vec2I size, Color color)
  : m_size(size)
  , m_pixels(compute_image_size(size))
  {
    uint8_t* ptr = m_pixels.data();
    const uint8_t r = to_byte(color.r);
    const uint8_t g = to_byte(color.g);
    const uint8_t b = to_byte(color.b);
    const uint8_t a = to_byte(color.a);

    for (int y = 0; y < m_size.h; ++y) {
      for (int x = 0; x < m_size.w; ++x) {
        ptr[0] = r;
        ptr[1] = g;
        ptr[2] = b;
        ptr[3] = a;
        ptr += 4;
      }
    }
  }

  Image::Image(Vec2I size, const uint8_t* pixels, PixelFormat format)
  : m_size(size)
  , m_pixels(compute_image_size(size))
  {
    assert(pixels != nullptr);

    switch (format) {
      case PixelFormat::Rgb24:
        {
          uint8_t* ptr = m_pixels.data();

          for (int y = 0; y < m_size.h; ++y) {
            for (int x = 0; x < m_size.w; ++x) {
              ptr[0] = pixels[0];
              ptr[1] = pixels[1];
              ptr[2] = pixels[2];
              ptr[3] = 0xFF; // set alpha to max (opaque)
              ptr += 4;
              pixels += 3;
            }
          }

          break;
        }

      case PixelFormat::Rgba32:
        std::copy_n(pixels, m_pixels.size(), m_pixels.data());
        break;
    }
  }

  Image::Image(const std::filesystem::path& filename)
  : m_size(0, 0)
  {
    int n = 0;
    uint8_t* pixels = stbi_load(filename.string().c_str(), &m_size.w, &m_size.h, &n, STBI_rgb_alpha);

    if (m_size.w == 0 || m_size.h == 0 || pixels == nullptr) {
      Log::fatal("Could not load image from file '{}': {}\n", filename.string(), stbi_failure_reason());
    }

    m_pixels.resize(compute_image_size(m_size));
    std::copy_n(pixels, m_pixels.size(), m_pixels.data());
    stbi_image_free(pixels);
  }

  Image::Image(InputStream& stream)
  : m_size(0, 0)
  {
    stbi_io_callbacks callbacks;
    callbacks.read = &stb_callback_read;
    callbacks.skip = &stb_callback_skip;
    callbacks.eof = &stb_callback_eof;

    int n = 0;
    uint8_t* pixels = stbi_load_from_callbacks(&callbacks, &stream, &m_size.w, &m_size.h, &n, STBI_rgb_alpha); // NOLINT

    if (m_size.w == 0 || m_size.h == 0 || pixels == nullptr) {
      Log::fatal("Could not load image from stream: {}\n", stbi_failure_reason());
    }

    m_pixels.resize(compute_image_size(m_size));
    std::copy_n(pixels, m_pixels.size(), m_pixels.data());
    stbi_image_free(pixels);
  }

  Vec2I Image::size() const
  {
    return m_size;
  }

  PositionRange Image::position_range() const
  {
    return gf::position_range(m_size);
  }

  Color Image::operator()(Vec2I position) const
  {
    if (position.x < 0 || position.x >= m_size.w || position.y < 0 || position.y >= m_size.h) {
      return Transparent;
    }

    const uint8_t* ptr = m_pixels.data() + offset_from_position(position);
    return { to_float(ptr[0]), to_float(ptr[1]), to_float(ptr[2]), to_float(ptr[3]) };
  }

  void Image::put_pixel(Vec2I position, Color color)
  {
    if (position.x < 0 || position.x >= m_size.w || position.y < 0 || position.y >= m_size.h) {
      return;
    }

    uint8_t* ptr = m_pixels.data() + offset_from_position(position);
    ptr[0] = to_byte(color.r);
    ptr[1] = to_byte(color.g);
    ptr[2] = to_byte(color.b);
    ptr[3] = to_byte(color.a);
  }

  void Image::save_to_file(const std::filesystem::path& filename) const
  {
    if (m_size.w == 0 || m_size.h == 0 || m_pixels.empty()) {
      return;
    }

    std::string extension = filename.extension().string();

    for (char& c : extension) {
      c = std::tolower(c, std::locale::classic());
    }

    const std::string filename_string = filename.string();

    if (extension == ".png") {
      stbi_write_png(filename_string.c_str(), m_size.w, m_size.h, 4, m_pixels.data(), 0);
      return;
    }

    if (extension == ".bmp") {
      stbi_write_bmp(filename_string.c_str(), m_size.w, m_size.h, 4, m_pixels.data());
      return;
    }

    if (extension == ".tga") {
      stbi_write_tga(filename_string.c_str(), m_size.w, m_size.h, 4, m_pixels.data());
      return;
    }

    Log::error("Format not supported: '{}'\n", extension);
  }

  void Image::blit_to(RectI origin_region, Image& target_image, Vec2I target_offset) const
  {
    const Blit blit = compute_blit(origin_region, m_size, target_offset, target_image.size());

    if (blit.origin_region.empty()) {
      return;
    }

    for (const Vec2I offset : gf::position_range(blit.origin_region.extent)) {
      target_image.put_pixel(blit.target_offset + offset, (*this)(blit.origin_region.offset + offset));
    }
  }

  void Image::blit_to(Image& target_image, Vec2I target_offset) const
  {
    blit_to(RectI::from_size(m_size), target_image, target_offset);
  }

  Image Image::sub_image(RectI area) const
  {
    const RectI current_area = RectI::from_size(m_size);
    const std::optional<RectI> maybe_intersection = current_area.intersection(area);

    if (!maybe_intersection) {
      return {};
    }

    const Vec2I position = maybe_intersection->position();
    const Vec2I size = maybe_intersection->size();

    Image image(size);

    const uint8_t* src = m_pixels.data() + offset_from_position(position);
    uint8_t* dst = image.m_pixels.data();

    for (int y = 0; y < size.h; ++y) {
      std::copy_n(src, size.w, dst);
      src += static_cast<std::ptrdiff_t>(4 * m_size.w);
      dst += static_cast<std::ptrdiff_t>(4 * size.w);
    }

    return image;
  }

  std::size_t Image::raw_size() const
  {
    return static_cast<std::size_t>(m_size.w) * static_cast<std::size_t>(m_size.h) * 4;
  }

  const uint8_t* Image::raw_data() const
  {
    if (m_pixels.empty()) {
      return nullptr;
    }

    return m_pixels.data();
  }

  std::ptrdiff_t Image::offset_from_position(Vec2I position) const
  {
    return static_cast<std::ptrdiff_t>(position.x + (position.y * m_size.w)) * 4;
  }

  Vec2I image_size(const std::filesystem::path& filename)
  {
    const std::unique_ptr<std::FILE, FileCloseDeleter> file(std::fopen(filename.string().c_str(), "rb"));

    if (!file) {
      Log::fatal("Unknwon file: {}", filename.string());
    }

    Vec2I size = { 0, 0 };
    int n = 0;
    auto ok = stbi_info_from_file(file.get(), &size.x, &size.y, &n);

    if (ok == 0) {
      Log::fatal("Could not find the size of the image: {}", filename.string());
    }

    return size;
  }

} // namespace gf
