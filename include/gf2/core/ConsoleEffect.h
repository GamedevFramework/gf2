// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_EFFECT_H
#define GF_CONSOLE_EFFECT_H

#include <cstdint>

#include "Color.h"
#include "CoreApi.h"

namespace gf {

  enum class ConsoleEffectType : uint8_t {
    None,
    Set,
    Multiply,
    Lighten,
    Darken,
    Screen,
    ColorDodge,
    ColorBurn,
    Add,
    AddAlpha,
    Burn,
    Overlay,
    Alpha,
  };

  class GF_CORE_API ConsoleEffect {
  public:
    static constexpr ConsoleEffect none()
    {
      return ConsoleEffectType::None;
    }

    static constexpr ConsoleEffect set()
    {
      return ConsoleEffectType::Set;
    }

    static constexpr ConsoleEffect multiply()
    {
      return ConsoleEffectType::Multiply;
    }

    static constexpr ConsoleEffect lighten()
    {
      return ConsoleEffectType::Lighten;
    }

    static constexpr ConsoleEffect darken()
    {
      return ConsoleEffectType::Darken;
    }

    static constexpr ConsoleEffect screen()
    {
      return ConsoleEffectType::Screen;
    }

    static constexpr ConsoleEffect color_dodge()
    {
      return ConsoleEffectType::ColorDodge;
    }

    static constexpr ConsoleEffect color_burn()
    {
      return ConsoleEffectType::ColorBurn;
    }

    static constexpr ConsoleEffect add()
    {
      return ConsoleEffectType::Add;
    }

    static constexpr ConsoleEffect add_alpha(float alpha)
    {
      return { ConsoleEffectType::AddAlpha, alpha };
    }

    static constexpr ConsoleEffect burn()
    {
      return ConsoleEffectType::Burn;
    }

    static constexpr ConsoleEffect overlay()
    {
      return ConsoleEffectType::Overlay;
    }

    static constexpr ConsoleEffect alpha(float alpha)
    {
      return { ConsoleEffectType::Alpha, alpha };
    }

    constexpr ConsoleEffectType type() const
    {
      return m_type;
    }

    constexpr float alpha() const
    {
      return m_alpha;
    }

    Color compute_color(Color existing, Color current) const;

  private:
    constexpr ConsoleEffect(ConsoleEffectType type, float alpha = 0.0f)
    : m_type(type)
    , m_alpha(alpha)
    {
    }

    ConsoleEffectType m_type = ConsoleEffectType::None;
    float m_alpha = 0.0f;
  };

}

#endif // GF_CONSOLE_EFFECT_H
