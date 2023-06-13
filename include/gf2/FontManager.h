// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_MANAGER_H
#define GF_FONT_MANAGER_H

#include "CoreApi.h"

namespace gf {

  class GF_CORE_API FontManager {
  public:
    FontManager();
    FontManager(const FontManager&) = delete;
    FontManager(FontManager&& other) noexcept;
    ~FontManager();

    FontManager& operator=(const FontManager&) = delete;
    FontManager& operator=(FontManager&& other) noexcept;

    template<typename T>
    T library_as()
    {
      return static_cast<T>(m_library);
    }

    template<typename T>
    T stroker_as()
    {
      return static_cast<T>(m_stroker);
    }

  private:
    void* m_library = nullptr;
    void* m_stroker = nullptr;
  };

}

#endif // GF_FONT_MANAGER_H
