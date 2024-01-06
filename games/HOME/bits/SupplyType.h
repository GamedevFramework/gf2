#ifndef HOME_SUPPLY_TYPE_H
#define HOME_SUPPLY_TYPE_H

#include <cstdint>

#include <gf2/core/Color.h>

namespace home {

  enum class SupplyType : uint8_t {
    Energy,
    Metal,
    Oxygen,
  };

  inline gf::Color to_color(SupplyType type)
  {
    switch (type) {
      case SupplyType::Energy:
        return gf::Yellow;
      case SupplyType::Metal:
        return gf::Gray;
      case SupplyType::Oxygen:
        return gf::Cyan;
    }

    return gf::Red;
  }

}

#endif // HOME_SUPPLY_TYPE_H
