// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <filesystem>

#include <gf2/core/Color.h>
#include <gf2/core/Image.h>
#include <gf2/core/Vec2.h>

#include "config.h"
#include "gf2/core/Log.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path image_file = assets_directory / "panel_brown_corners_a.png";
  constexpr gf::Vec2I SourceImageSize = { 128, 128 };

  gf::Image source_image(image_file);
  gf::Image destination_image(SourceImageSize * 2, gf::Transparent);

  source_image.blit_to(destination_image, SourceImageSize * 0.5f);

  const std::filesystem::path output_image = std::filesystem::temp_directory_path() / "blit.png";
  destination_image.save_to_file(output_image);
  gf::Log::info("The image output path is {}", output_image.string());

  return 0;
}
