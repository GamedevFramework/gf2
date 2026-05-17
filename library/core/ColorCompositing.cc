// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ColorCompositing.h>

#include <gf2/core/Math.h>

namespace gf {
  /*
   * Sources:
   * - https://www.w3.org/TR/compositing/
   * - https://marius.sucan.ro/blog/2023/blending-modes-maths/
   */

  namespace {
    /*
     * Blending
     */

    constexpr Color mix_normalize(Color color)
    {
      color.a = 1.0f;
      return clamp(color);
    }

    constexpr float channel_color_dodge(float source, float backdrop)
    {
      if (backdrop == 0.0f) {
        return 0.0f;
      }

      if (source == 1.0f) {
        return 1.0f;
      }

      return details::min(1.0f, backdrop / (1.0f - source));
    }

    constexpr float channel_color_burn(float source, float backdrop)
    {
      if (backdrop == 1.0f) {
        return 1.0f;
      }

      if (source == 0.0f) {
        return 0.0f;
      }

      return 1.0f - details::min(1.0f, (1.0f - backdrop) / source);
    }

    constexpr float channel_overlay(float source, float backdrop)
    {
      if (source < 0.5f) {
        return 2 * source * backdrop;
      }

      return 1.0f - (2 * (1.0f - source) * (1.0f - backdrop));
    }

    constexpr float channel_hard_light(float source, float backdrop)
    {
      if (backdrop <= 0.5f) {
        return 2 * source * backdrop;
      }

      return 1.0f - (2 * (1.0f - source) * (1.0f - backdrop));
    }

    inline float channel_soft_light(float source, float backdrop)
    {
      if (source <= 0.5f) {
        return backdrop - ((1.0f - (2.0f * source)) * backdrop * (1.0f - backdrop));
      }

      const float d = backdrop <= 0.25f ? ((((16.0f * backdrop) - 12.0f) * backdrop) + 4.0f) * backdrop : std::sqrt(backdrop);
      return backdrop + (((2.0f * source) - 1.0f) * (d - backdrop));
    }

    constexpr float channel_vivid_light(float source, float backdrop)
    {
      if (source <= 0.5f) {
        return 1.0f - ((1.0f - backdrop) / (2.0f * source));
      }

      return backdrop / (2.0f * (1.0f - source));
    }

    constexpr float channel_hard_mix(float source, float backdrop)
    {
      if (source <= 1.0f - backdrop) {
        return 0.0f;
      }

      return 1.0f;
    }

    Color compute_raw_mixed_color(Color source, Color backdrop, BlendMode mode)
    {
      switch (mode) {
        case BlendMode::Normal:
          return source;
        case BlendMode::Darken:
          return min(backdrop, source);
        case BlendMode::Multiply:
          return backdrop * source;
        case BlendMode::ColorBurn:
          return {
            channel_color_burn(backdrop.r, source.r),
            channel_color_burn(backdrop.g, source.g),
            channel_color_burn(backdrop.b, source.b)
          };
        case BlendMode::LinearBurn:
          return backdrop + source - White;
        case BlendMode::Lighten:
          return max(backdrop, source);
        case BlendMode::Screen:
          return 1.0f - (1.0f - backdrop) * (1.0f - source);
        case BlendMode::ColorDodge:
          return {
            channel_color_dodge(backdrop.r, source.r),
            channel_color_dodge(backdrop.g, source.g),
            channel_color_dodge(backdrop.b, source.b)
          };
        case BlendMode::LinearDodge:
          return backdrop + source;
        case BlendMode::Overlay:
          return {
            channel_overlay(backdrop.r, source.r),
            channel_overlay(backdrop.g, source.g),
            channel_overlay(backdrop.b, source.b)
          };
        case BlendMode::SoftLight:
          return {
            channel_soft_light(backdrop.r, source.r),
            channel_soft_light(backdrop.g, source.g),
            channel_soft_light(backdrop.b, source.b)
          };
        case BlendMode::HardLight:
          return {
            channel_hard_light(backdrop.r, source.r),
            channel_hard_light(backdrop.g, source.g),
            channel_hard_light(backdrop.b, source.b)
          };
        case BlendMode::VividLight:
          return {
            channel_vivid_light(backdrop.r, source.r),
            channel_vivid_light(backdrop.g, source.g),
            channel_vivid_light(backdrop.b, source.b)
          };
        case BlendMode::LinearLight:
          return backdrop + 2.0f *source - White;
        case BlendMode::HardMix:
          return {
            channel_hard_mix(backdrop.r, source.r),
            channel_hard_mix(backdrop.g, source.g),
            channel_hard_mix(backdrop.b, source.b)
          };
        case BlendMode::Difference:
          return abs(source - backdrop);
        case BlendMode::Exclusion:
          return source + backdrop - (2.0f * source * backdrop);
        case BlendMode::Substract:
          return backdrop - source;
        case BlendMode::Divide:
          return clamp(backdrop / source);
      }

      assert(false);
      return Black;
    }

    /*
     * Compositing
     */

    constexpr Color compute_porter_duff_equation(Color source, Color backdrop, float fa, float fb)
    {
      const float alpha = (source.a * fa) + (backdrop.a * fb);

      if (alpha < Epsilon) {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
      }

      return {
        ((source.a * fa * source.r) + (backdrop.a * fb * backdrop.r)) / alpha,
        ((source.a * fa * source.g) + (backdrop.a * fb * backdrop.g)) / alpha,
        ((source.a * fa * source.b) + (backdrop.a * fb * backdrop.b)) / alpha,
        alpha,
      };
    }

    Color compute_raw_composed_color(Color source, Color backdrop, CompositingOperation operation)
    {
      switch (operation) {
        case CompositingOperation::Clear:
          return { 0.0f, 0.0f, 0.0f, 0.0f };
        case CompositingOperation::Source:
          return compute_porter_duff_equation(source, backdrop, 1.0f, 0.0f);
        case CompositingOperation::Destination:
          return compute_porter_duff_equation(source, backdrop, 0.0f, 1.0f);
        case CompositingOperation::SourceOver:
          return compute_porter_duff_equation(source, backdrop, 1.0f, 1.0f - source.a);
        case CompositingOperation::DestinationOver:
          return compute_porter_duff_equation(source, backdrop, 1.0f - backdrop.a, 1.0f);
        case CompositingOperation::SourceIn:
          return compute_porter_duff_equation(source, backdrop, backdrop.a, 0.0f);
        case CompositingOperation::DestinationIn:
          return compute_porter_duff_equation(source, backdrop, 0.0f, source.a);
        case CompositingOperation::SourceOut:
          return compute_porter_duff_equation(source, backdrop, 1.0f - backdrop.a, 0.0f);
        case CompositingOperation::DestinationOut:
          return compute_porter_duff_equation(source, backdrop, 0.0f, 1.0f - source.a);
        case CompositingOperation::SourceAtop:
          return compute_porter_duff_equation(source, backdrop, backdrop.a, 1.0f - source.a);
        case CompositingOperation::DestinationAtop:
          return compute_porter_duff_equation(source, backdrop, 1.0f - backdrop.a, source.a);
        case CompositingOperation::Xor:
          return compute_porter_duff_equation(source, backdrop, 1.0f - backdrop.a, 1.0f - source.a);
      }

      assert(false);
      return Black;
    }

  }

  Color mix_colors(Color source, Color backdrop, BlendMode mode)
  {
    return mix_normalize(compute_raw_mixed_color(source, backdrop, mode));
  }

  Color compose_colors(Color source, Color backdrop, CompositingOperation operation)
  {
    return clamp(compute_raw_composed_color(source, backdrop, operation));
  }

  Color combine_colors(Color source, Color backdrop, BlendMode mode, CompositingOperation operation)
  {
    Color mixed = clamp(compute_raw_mixed_color(source, backdrop, mode));
    mixed.a = source.a;
    return clamp(compute_raw_composed_color(mixed, backdrop, operation));
  }

  Color blend_colors(Color source, Color backdrop, BlendMode mode) {
    return combine_colors(source, backdrop, mode, CompositingOperation::SourceOver);
  }

}
