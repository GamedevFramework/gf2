// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_EFFECT_H
#define GF_CONSOLE_EFFECT_H

#include <cstdint>

#include "Color.h"
#include "ColorCompositing.h"
#include "CoreApi.h"

namespace gf {

  class GF_CORE_API ConsoleEffect {
  public:
    constexpr ConsoleEffect(BlendMode mode = BlendMode::Normal, CompositingOperation operation = CompositingOperation::Source)
    : m_mode(mode)
    , m_operation(operation)
    {
    }

    static constexpr ConsoleEffect none()
    {
      return { BlendMode::Normal, CompositingOperation::Destination };
    }

    static constexpr ConsoleEffect set()
    {
      return BlendMode::Normal;
    }

    static constexpr ConsoleEffect multiply()
    {
      return BlendMode::Multiply;
    }

    static constexpr ConsoleEffect lighten()
    {
      return BlendMode::Lighten;
    }

    static constexpr ConsoleEffect darken()
    {
      return BlendMode::Darken;
    }

    static constexpr ConsoleEffect screen()
    {
      return BlendMode::Screen;
    }

    static constexpr ConsoleEffect color_dodge()
    {
      return BlendMode::ColorDodge;
    }

    static constexpr ConsoleEffect color_burn()
    {
      return BlendMode::ColorBurn;
    }

    static constexpr ConsoleEffect add()
    {
      return BlendMode::LinearDodge;
    }

    static constexpr ConsoleEffect linear_dodge()
    {
      return BlendMode::LinearDodge;
    }

    static constexpr ConsoleEffect linear_burn()
    {
      return BlendMode::LinearBurn;
    }

    static constexpr ConsoleEffect overlay()
    {
      return BlendMode::Overlay;
    }

    static constexpr ConsoleEffect alpha()
    {
      return { BlendMode::Normal, CompositingOperation::SourceOver };
    }

    constexpr BlendMode blend_mode() const
    {
      return m_mode;
    }

    constexpr CompositingOperation compositing_operation() const
    {
      return m_operation;
    }

    Color compute_color(Color existing, Color proposed) const;

  private:
    BlendMode m_mode;
    CompositingOperation m_operation;
  };

}

#endif // GF_CONSOLE_EFFECT_H
