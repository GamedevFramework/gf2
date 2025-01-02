// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Time.h>

#include <gf2/core/Serialization.h>

namespace gf {

  Serializer& operator|(Serializer& archive, Time time)
  {
    archive.write_i64(time.as_microseconds());
    return archive;
  }

  Deserializer& operator|(Deserializer& archive, Time& time)
  {
    int64_t value = 0;
    archive.read_i64(&value);
    time = gf::microseconds(value);
    return archive;
  }

}
