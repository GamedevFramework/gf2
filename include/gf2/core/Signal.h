// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SIGNAL_H
#define GF_SIGNAL_H

#include <cstdint>
#include <cassert>

#include <functional>
#include <vector>

namespace gf {

  enum class SignalId : uint64_t;

  template<typename Signature>
  class Signal;

  template<typename... Args>
  class Signal<void(Args...)> {
  public:

    template<typename Func>
    SignalId connect(Func&& callback)
    {
      SignalId id = SignalId{m_slots.size()};
      m_slots.emplace_back(std::forward<Func>(callback));
      return id;
    }

    void disconnect(SignalId id)
    {
      auto index = static_cast<std::size_t>(id);
      assert(index < m_slots.size());
      m_slots[index] = nullptr;
    }

    void emit(Args... args)
    {
      for (auto& slot : m_slots) {
        if (slot == nullptr) {
          continue;
        }

        slot(args...);
      }
    }

  private:
    std::vector<std::function<void(Args...)>> m_slots;
  };

}

#endif // GF_SIGNAL_H
