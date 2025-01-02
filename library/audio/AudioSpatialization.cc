// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/audio/AudioSpatialization.h>

#include "bits/AudioBackend.h"

namespace gf {

  static_assert(static_cast<uint8_t>(AudioPositioning::Absolute) == ma_positioning_absolute, "Check AudioPositioning");
  static_assert(static_cast<uint8_t>(AudioPositioning::Relative) == ma_positioning_relative, "Check AudioPositioning");

}
