// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ConsoleEffect.h>

#include <cassert>

namespace gf {

  namespace {

    constexpr float channel_color_dodge(float existing, float current)
    {
      if (existing != 1.0f) {
        return current / (1.0f - existing);
      }

      return 1.0f;
    }

    constexpr float channel_color_burn(float existing, float current)
    {
      if (existing != 0.0f) {
        return (1.0f - current) / existing;
      }

      return 0.0f;
    }

    constexpr float channel_overlay(float existing, float current)
    {
      if (current < 0.5f) {
        return 2 * current * existing;
      }

      return 1.0f - 2 * (1.0f - current) * (1.0f - existing);
    }

  }

  Color ConsoleEffect::compute_color(Color existing, Color current) const
  {
    Color result = Black;

    switch (m_type) {
      case ConsoleEffectType::None:
        return existing;

      case ConsoleEffectType::Set:
        return current;

      case ConsoleEffectType::Multiply:
        return existing * current;

      case ConsoleEffectType::Lighten:
        return gf::max(existing, current);

      case ConsoleEffectType::Darken:
        return gf::min(existing, current);

      case ConsoleEffectType::Screen:
        return 1.0f - (1.0f - existing) * (1.0f - current);

      case ConsoleEffectType::ColorDodge:
        result.r = channel_color_dodge(existing.r, current.r);
        result.g = channel_color_dodge(existing.g, current.b);
        result.b = channel_color_dodge(existing.b, current.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::ColorBurn:
        result.r = channel_color_burn(existing.r, current.r);
        result.g = channel_color_burn(existing.g, current.b);
        result.b = channel_color_burn(existing.b, current.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::Add:
        return gf::clamp(existing + current, 0.0f, 1.0f);

      case ConsoleEffectType::AddAlpha:
        return gf::clamp(existing + m_alpha * current, 0.0f, 1.0f);

      case ConsoleEffectType::Burn:
        return gf::clamp(existing + current - White, 0.0f, 1.0f);

      case ConsoleEffectType::Overlay:
        result.r = channel_overlay(existing.r, current.r);
        result.g = channel_overlay(existing.g, current.b);
        result.b = channel_overlay(existing.b, current.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::Alpha:
        return gf::lerp(existing, current, m_alpha);
    }

    assert(false);
    return Black;
  }

}
