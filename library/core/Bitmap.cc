// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Bitmap.h>

#include <algorithm>

#include <gf2/core/Blit.h>
#include <gf2/core/Image.h>

namespace gf {

  namespace {
    std::size_t compute_bitmap_size(Vec2I size)
    {
      return static_cast<std::size_t>(size.w) * static_cast<std::size_t>(size.h);
    }
  }

  Bitmap::Bitmap()
  : m_size(0, 0)
  {
  }

  Bitmap::Bitmap(Vec2I size)
  : m_size(size)
  , m_pixels(compute_bitmap_size(size), 0xFF)
  {
  }

  Bitmap::Bitmap(Vec2I size, uint8_t gray)
  : m_size(size)
  , m_pixels(compute_bitmap_size(size), gray)
  {
  }

  Bitmap::Bitmap(Vec2I size, const uint8_t* source, ptrdiff_t pitch)
  : m_size(size)
  , m_pixels(compute_bitmap_size(size))
  {
    uint8_t* target = m_pixels.data();

    if (pitch == static_cast<ptrdiff_t>(m_size.w)) {
      std::copy_n(source, compute_bitmap_size(size), target);
    } else {
      for (int row = 0; row < size.y; ++row) {
        std::copy_n(source, size.w, target);
        source += pitch;
        target += size.w;
      }
    }
  }

  Vec2I Bitmap::size() const
  {
    return m_size;
  }

  PositionRange Bitmap::position_range() const
  {
    return gf::position_range(m_size);
  }

  uint8_t Bitmap::operator()(Vec2I position) const
  {
    if (position.x < 0 || position.x >= m_size.w || position.y < 0 || position.y >= m_size.h) {
      return 0xFF;
    }

    return m_pixels[offset_from_position(position)];
  }

  void Bitmap::put_pixel(Vec2I position, uint8_t gray)
  {
    if (position.x < 0 || position.x >= m_size.w || position.y < 0 || position.y >= m_size.h) {
      return;
    }

    m_pixels[offset_from_position(position)] = gray;
  }

  void Bitmap::save_to_file(const std::filesystem::path& filename) const
  {
    Image image(m_size);

    for (const Vec2I position : position_range()) {
      image.put_pixel(position, White * opaque(static_cast<float>((*this)(position)) / 255.0f));
    }

    image.save_to_file(filename);
  }

  void Bitmap::blit_to(RectI origin_region, Bitmap& target_bitmap, Vec2I target_offset) const
  {
    const Blit blit = compute_blit(origin_region, m_size, target_offset, target_bitmap.size());

    if (blit.origin_region.empty()) {
      return;
    }

    for (const Vec2I offset : gf::position_range(blit.origin_region.extent)) {
      target_bitmap.put_pixel(blit.target_offset + offset, (*this)(blit.origin_region.offset + offset));
    }
  }

  void Bitmap::blit_to(Bitmap& target_bitmap, Vec2I target_offset) const
  {
    blit_to(RectI::from_size(m_size), target_bitmap, target_offset);
  }

  Bitmap Bitmap::sub_bitmap(RectI area) const
  {
    const RectI current_area = RectI::from_size(m_size);
    const std::optional<RectI> maybe_intersection = current_area.intersection(area);

    if (!maybe_intersection) {
      return {};
    }

    const Vec2I position = maybe_intersection->position();
    const Vec2I size = maybe_intersection->size();

    Bitmap bitmap(size);

    const uint8_t* src = m_pixels.data() + offset_from_position(position);
    uint8_t* dst = bitmap.m_pixels.data();

    for (int y = 0; y < size.h; ++y) {
      std::copy_n(src, size.w, dst);
      src += static_cast<std::ptrdiff_t>(m_size.w);
      dst += static_cast<std::ptrdiff_t>(size.w);
    }

    return bitmap;
  }

  std::size_t Bitmap::raw_size() const
  {
    return static_cast<std::size_t>(m_size.w) * static_cast<std::size_t>(m_size.h);
  }

  const uint8_t* Bitmap::raw_data() const
  {
    if (m_pixels.empty()) {
      return nullptr;
    }

    return m_pixels.data();
  }

  std::ptrdiff_t Bitmap::offset_from_position(Vec2I position) const
  {
    return static_cast<std::ptrdiff_t>(position.x) + (static_cast<std::ptrdiff_t>(position.y) * static_cast<std::ptrdiff_t>(m_size.w));
  }

}
