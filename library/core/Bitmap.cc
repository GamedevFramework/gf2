// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Bitmap.h>

#include <algorithm>
#include <fstream>

#include <gf2/core/Blit.h>

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
    auto* target = m_pixels.data();

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

  void Bitmap::blit(RectI source_region, const Bitmap& source, Vec2I target_offset)
  {
    auto blit = compute_blit(source_region, source.size(), target_offset, m_size);

    if (blit.source_region.empty()) {
      return;
    }

    for (auto offset : gf::position_range(blit.source_region.extent)) {
      put_pixel(blit.target_offset + offset, source(blit.source_region.offset + offset));
    }
  }

  void Bitmap::blit(const Bitmap& source, Vec2I target_offset)
  {
    blit(RectI::from_size(source.m_size), source, target_offset);
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

  void Bitmap::export_to(const std::filesystem::path& filename) const
  {
    std::ofstream file(filename);
    file << "P7\nWIDTH " << m_size.w << "\nHEIGHT " << m_size.h << "\nDEPTH 1\nMAXVAL 255\nTUPLTYPE GRAYSCALE\nENDHDR\n";

    for (const uint8_t value : m_pixels) {
      file << value;
    }
  }

  std::ptrdiff_t Bitmap::offset_from_position(Vec2I position) const
  {
    return static_cast<std::ptrdiff_t>(position.x) + static_cast<std::ptrdiff_t>(position.y) * static_cast<std::ptrdiff_t>(m_size.w);
  }

}
