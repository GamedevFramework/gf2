// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ACTION_GROUP_H
#define GF_ACTION_GROUP_H

#include <string_view>

#include <gf2/core/ActionSettings.h>
#include <gf2/core/Id.h>

#include "Action.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API ActionGroup {
  public:
    ActionGroup() = default;
    ActionGroup(const ActionGroupSettings& settings);

    void add_action(std::string_view name, const ActionSettings& settings);
    void add_action(Id id, const ActionSettings& settings);
    void add_action(std::string_view name, Action action);
    void add_action(Id id, Action action);

    bool active(std::string_view name) const;
    bool active(Id id) const;

    void reset();
    void process_event(const Event& event);

  private:
    std::map<Id, Action> m_actions;
  };

}

#endif // GF_ACTION_GROUP_H
