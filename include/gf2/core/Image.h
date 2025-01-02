// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_IMAGE_H
#define GF_IMAGE_H

#include <cstdint>

#include <filesystem>
#include <vector>

#include "Color.h"
#include "Range.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {
  class InputStream;

  enum class PixelFormat : uint8_t {
    Rgba32,
    Rgb24,
  };

  class GF_CORE_API Image {
  public:
    Image();
    Image(Vec2I size);
    Image(Vec2I size, Color color);
    Image(Vec2I size, const uint8_t* pixels, PixelFormat format = PixelFormat::Rgba32);
    Image(const std::filesystem::path& filename);
    Image(InputStream& stream);

    Vec2I size() const;
    PositionRange position_range() const;

    Color operator()(Vec2I position) const;
    void put_pixel(Vec2I position, Color color);

    void save_to_file(const std::filesystem::path& filename) const;

    Image sub_image(RectI area) const;

    std::size_t raw_size() const;
    const uint8_t* raw_data() const;

  private:
    std::ptrdiff_t offset_from_position(Vec2I position) const;

    Vec2I m_size;
    std::vector<uint8_t> m_pixels;
  };

  GF_CORE_API Vec2I image_size(const std::filesystem::path& filename);

} // namespace gf

#endif // GF_IMAGE_H
