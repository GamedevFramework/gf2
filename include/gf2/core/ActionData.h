// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ACTION_DATA_H
#define GF_ACTION_DATA_H

#include <cstdint>

#include <map>
#include <vector>

#include "ControlData.h"
#include "CoreApi.h"
#include "Id.h"
#include "TypeTraits.h"

namespace gf {

  enum class ActionType : uint8_t {
    Instantaneous,
    Continuous,
  };

  struct GF_CORE_API ActionData {
    ActionType type;
    std::vector<ControlData> controls;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionData, Archive>& data)
  {
    return ar | data.type | data.controls;
  }

  struct GF_CORE_API ActionGroupData {
    std::map<Id, ActionData> actions;
  };

  template<typename Archive>
  inline Archive& operator|(Archive& ar, MaybeConst<ActionGroupData, Archive>& data)
  {
    return ar | data.actions;
  }

}

#endif // GF_ACTION_DATA_H
