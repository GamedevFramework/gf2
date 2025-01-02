// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_EASING_H
#define GF_EASING_H

#include "CoreApi.h"

namespace gf {

  using Easing = float (*)(float);

  GF_CORE_API float ease_linear(float t);
  GF_CORE_API float ease_smooth(float t);
  GF_CORE_API float ease_smoother(float t);

  GF_CORE_API float ease_in_sine(float t);
  GF_CORE_API float ease_out_sine(float t);
  GF_CORE_API float ease_in_out_sine(float t);
  GF_CORE_API float ease_out_in_sine(float t);

  GF_CORE_API float ease_in_quad(float t);
  GF_CORE_API float ease_out_quad(float t);
  GF_CORE_API float ease_in_out_quad(float t);
  GF_CORE_API float ease_out_in_quad(float t);

  GF_CORE_API float ease_in_cubic(float t);
  GF_CORE_API float ease_out_cubic(float t);
  GF_CORE_API float ease_in_out_cubic(float t);
  GF_CORE_API float ease_out_in_cubic(float t);

  GF_CORE_API float ease_in_quart(float t);
  GF_CORE_API float ease_out_quart(float t);
  GF_CORE_API float ease_in_out_quart(float t);
  GF_CORE_API float ease_out_in_quart(float t);

  GF_CORE_API float ease_in_quint(float t);
  GF_CORE_API float ease_out_quint(float t);
  GF_CORE_API float ease_in_out_quint(float t);
  GF_CORE_API float ease_out_in_quint(float t);

  GF_CORE_API float ease_in_circ(float t);
  GF_CORE_API float ease_out_circ(float t);
  GF_CORE_API float ease_in_out_circ(float t);
  GF_CORE_API float ease_out_in_circ(float t);

  GF_CORE_API float ease_in_back(float t);
  GF_CORE_API float ease_out_back(float t);
  GF_CORE_API float ease_in_out_back(float t);
  GF_CORE_API float ease_out_in_back(float t);

  GF_CORE_API float ease_in_bounce(float t);
  GF_CORE_API float ease_out_bounce(float t);
  GF_CORE_API float ease_in_out_bounce(float t);
  GF_CORE_API float ease_out_in_bounce(float t);

  GF_CORE_API float ease_in_elastic(float t);
  GF_CORE_API float ease_out_elastic(float t);
  GF_CORE_API float ease_in_out_elastic(float t);
  GF_CORE_API float ease_out_in_elastic(float t);

  GF_CORE_API float ease_in_expo(float t);
  GF_CORE_API float ease_out_expo(float t);
  GF_CORE_API float ease_in_out_expo(float t);
  GF_CORE_API float ease_out_in_expo(float t);

}

#endif // GF_EASING_H
