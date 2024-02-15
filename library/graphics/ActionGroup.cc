// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/ActionGroup.h>

#include <cassert>

#include <gf2/core/Log.h>

namespace gf {
  ActionGroup::ActionGroup(const ActionGroupData& data)
  {
    for (const auto& [id, action] : data.actions) {
      add_action(id, action);
    }
  }

  void ActionGroup::add_action(std::string_view name, const ActionData& data)
  {
    m_actions.emplace(gf::hash(name), Action(data));
  }

  void ActionGroup::add_action(Id id, const ActionData& data)
  {
    m_actions.emplace(id, Action(data));
  }

  void ActionGroup::add_action(std::string_view name, Action action)
  {
    m_actions.emplace(gf::hash(name), std::move(action));
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
      const auto& [actual_id, action] = *iterator;
      assert(id == actual_id);
      return action.active();
    }

    Log::warning("Unknown action id: {:X}.", static_cast<uint64_t>(id));
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
