// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/ActionGroup.h>
// clang-format on

namespace gf {

  void ActionGroup::add_action(std::string_view name, Action action)
  {
    add_action(gf::hash(name), std::move(action));
  }

  void ActionGroup::add_action(Id id, Action action)
  {
    m_actions.emplace(id, std::move(action));
  }

  bool ActionGroup::active(std::string_view name) const
  {
    return active(gf::hash(name));
  }

  bool ActionGroup::active(Id id) const
  {
    if (auto iterator = m_actions.find(id); iterator != m_actions.end()) {
      const auto& [id, action] = *iterator;
      return action.active();
    }

    // TODO: warning?
    return false;
  }

  void ActionGroup::reset()
  {
    for (auto& [id, action] : m_actions) {
      action.reset();
    }
  }

  void ActionGroup::process_event(const Event& event)
  {
    for (auto& [id, action] : m_actions) {
      action.process_event(event);
    }
  }

}
