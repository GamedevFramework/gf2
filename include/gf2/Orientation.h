#ifndef GF_ORIENTATION_H
#define GF_ORIENTATION_H

#include <cstdint>

#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  enum class Orientation : int8_t {
    Center    = -1, ///< The center, indicates no orientation
    North     =  0, ///< The north orientation
    NorthEast =  1, ///< The north-east orientation
    East      =  2, ///< The east orientation
    SouthEast =  3, ///< The south-east orientation
    South     =  4, ///< The south orientation
    SouthWest =  5, ///< The south-west orientation
    West      =  6, ///< The west orientation
    NorthWest =  7, ///< The north-west orientation
  };

  GF_CORE_API Vec2F unit(Orientation orientation);
  GF_CORE_API Vec2I displacement(Orientation orientation);
  GF_CORE_API float angle(Orientation orientation);
  GF_CORE_API Orientation orientation(float angle);
  GF_CORE_API Orientation opposite(Orientation orientation);

}

#endif // GF_ORIENTATION_H
