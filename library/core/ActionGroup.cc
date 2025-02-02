// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ActionGroup.h>

#include <cassert>

#include <gf2/core/Log.h>

namespace gf {
  ActionGroup::ActionGroup(const ActionGroupSettings& settings)
  {
    for (const auto& [id, action] : settings.actions) {
      add_action(id, action);
    }
  }

  void ActionGroup::add_action(std::string_view name, const ActionSettings& settings)
  {
    m_actions.emplace(gf::hash_string(name), Action(settings));
  }

  void ActionGroup::add_action(Id id, const ActionSettings& settings)
  {
    m_actions.emplace(id, Action(settings));
  }

  void ActionGroup::add_action(std::string_view name, Action action)
  {
    m_actions.emplace(gf::hash_string(name), std::move(action));
  }

  void ActionGroup::add_action(Id id, Action action)
  {
    m_actions.emplace(id, std::move(action));
  }

  bool ActionGroup::active(std::string_view name) const
  {
    return active(gf::hash_string(name));
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

  void ActionGroup::hard_reset()
  {
    for (auto& [id, action] : m_actions) {
      action.hard_reset();
    }
  }

  void ActionGroup::process_event(const Event& event)
  {
    for (auto& [id, action] : m_actions) {
      action.process_event(event);
    }
  }

}
