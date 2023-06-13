// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/FontManager.h>
// clang-format on

#include <cassert>

#include <utility>

// clang-format off
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// clang-format on

#include <gf2/Log.h>

namespace gf {

  namespace {

    const char* freetype_error_message(FT_Error error)
    {
      switch (error) {

#undef FTERRORS_H_
// NOLINTNEXTLINE
#define FT_ERRORDEF(e, v, s) \
  case v:                    \
    return s;
#include FT_ERRORS_H

        default:
          break;
      };

      return "unknown error";
    }

  }

  FontManager::FontManager()
  {
    FT_Library library = nullptr;

    if (auto err = FT_Init_FreeType(&library)) {
      Log::error("Could not init Freetype library: {}", freetype_error_message(err));
      throw std::runtime_error("Could not init Freetype library");
    }

    m_library = library;

    FT_Stroker stroker = nullptr;

    if (auto err = FT_Stroker_New(library, &stroker)) {
      Log::error("Could not create the stroker: {}", freetype_error_message(err));
      throw std::runtime_error("Could not create the stroker");
    }

    m_stroker = stroker;
  }

  FontManager::FontManager(FontManager&& other) noexcept
  : m_library(std::exchange(other.m_library, nullptr))
  , m_stroker(std::exchange(other.m_stroker, nullptr))
  {
  }

  FontManager::~FontManager()
  {
    if (m_stroker != nullptr) {
      // no possible error for this call
      FT_Stroker_Done(stroker_as<FT_Stroker>());
    }

    if (m_library != nullptr) {
      [[maybe_unused]] auto err = FT_Done_FreeType(library_as<FT_Library>());
      assert(err == FT_Err_Ok);
    }
  }

  FontManager& FontManager::operator=(FontManager&& other) noexcept
  {
    std::swap(other.m_library, m_library);
    std::swap(other.m_stroker, m_stroker);
    return *this;
  }

}
