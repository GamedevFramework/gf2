// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/FontManager.h>
// clang-format on

#include <cassert>

#include <utility>

// clang-format off
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_MODULE_H
// clang-format on

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    const char* ft_error_message(FT_Error error)
    {
      switch (error) {

#undef FTERRORS_H_
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
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
      Log::fatal("Could not init Freetype library: {}", ft_error_message(err));
    }

    m_library = library;

    FT_Stroker stroker = nullptr;

    if (auto err = FT_Stroker_New(library, &stroker)) {
      Log::fatal("Could not create the stroker: {}", ft_error_message(err));
    }

    m_stroker = stroker;

    const FT_Int spread = 8; // normally, the default value, but in case.

    if (auto err = FT_Property_Set(library, "bsdf", "spread", &spread)) {
      Log::warning("Could not set spread: {}", ft_error_message(err));
    }
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

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GF_XSTRINGIFY(X) #X
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GF_STRINGIFY(X) GF_XSTRINGIFY(X)

  std::string FontManager::backend()
  {
    FT_Int major = 0;
    FT_Int minor = 0;
    FT_Int patch = 0;
    FT_Library_Version(library_as<FT_Library>(), &major, &minor, &patch);

    using namespace std::string_literals;
    return "FreeType "s + std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(patch) + " (" GF_STRINGIFY(FREETYPE_MAJOR) "." GF_STRINGIFY(FREETYPE_MINOR) "." GF_STRINGIFY(FREETYPE_PATCH) ")";
  }

#undef GF_STRINGIFY
#undef GF_XSTRINGIFY

  const char* FontManager::error_message(int error)
  {
    return ft_error_message(static_cast<FT_Error>(error));
  }

}
