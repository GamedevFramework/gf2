// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ConsoleEffect.h>

#include <cassert>

namespace gf {

  namespace {

    constexpr float channel_color_dodge(float existing, float proposed)
    {
      if (existing != 1.0f) {
        return proposed / (1.0f - existing);
      }

      return 1.0f;
    }

    constexpr float channel_color_burn(float existing, float proposed)
    {
      if (existing != 0.0f) {
        return (1.0f - proposed) / existing;
      }

      return 0.0f;
    }

    constexpr float channel_overlay(float existing, float proposed)
    {
      if (proposed < 0.5f) {
        return 2 * proposed * existing;
      }

      return 1.0f - (2 * (1.0f - proposed) * (1.0f - existing));
    }

  }

  Color ConsoleEffect::compute_color(Color existing, Color proposed) const
  {
    Color result = Black;

    switch (m_type) {
      case ConsoleEffectType::None:
        return existing;

      case ConsoleEffectType::Set:
        return proposed;

      case ConsoleEffectType::Multiply:
        return existing * proposed;

      case ConsoleEffectType::Lighten:
        return gf::max(existing, proposed);

      case ConsoleEffectType::Darken:
        return gf::min(existing, proposed);

      case ConsoleEffectType::Screen:
        return 1.0f - (1.0f - existing) * (1.0f - proposed);

      case ConsoleEffectType::ColorDodge:
        result.r = channel_color_dodge(existing.r, proposed.r);
        result.g = channel_color_dodge(existing.g, proposed.b);
        result.b = channel_color_dodge(existing.b, proposed.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::ColorBurn:
        result.r = channel_color_burn(existing.r, proposed.r);
        result.g = channel_color_burn(existing.g, proposed.b);
        result.b = channel_color_burn(existing.b, proposed.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::Add:
        return gf::clamp(existing + proposed, 0.0f, 1.0f);

      case ConsoleEffectType::AddAlpha:
        return gf::clamp(existing + m_alpha * proposed, 0.0f, 1.0f);

      case ConsoleEffectType::Burn:
        return gf::clamp(existing + proposed - White, 0.0f, 1.0f);

      case ConsoleEffectType::Overlay:
        result.r = channel_overlay(existing.r, proposed.r);
        result.g = channel_overlay(existing.g, proposed.b);
        result.b = channel_overlay(existing.b, proposed.b);
        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffectType::Alpha:
        return gf::lerp(existing, proposed, m_alpha);
    }

    assert(false);
    return Black;
  }

}
