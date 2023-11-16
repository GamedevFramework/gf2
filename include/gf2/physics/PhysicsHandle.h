// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_HANDLE_H
#define GF_PHYSICS_HANDLE_H

#include <cassert>
#include <cstddef>

#include <utility>

namespace gf::details {

  class PhysicsExistingType {
  };

  constexpr PhysicsExistingType PhysicsExisting = {};

  template<typename T, void* (*UserDataGetter)(const T*), void (*UserDataSetter)(T*, void*), void (*Destructor)(T*)>
  class PhysicsHandle {
  public:
    using CounterType = std::size_t;

    PhysicsHandle()
    : m_handle(nullptr)
    {
    }

    PhysicsHandle(T* handle)
    : m_handle(handle)
    {
      assert(user_data() == nullptr);
      UserDataSetter(m_handle, new CounterType(0));
      reference();
    }

    PhysicsHandle([[maybe_unused]] PhysicsExistingType existing, T* handle)
    : m_handle(handle)
    {
      assert(user_data() != nullptr);
      reference();
    }

    PhysicsHandle(const PhysicsHandle& other)
    : m_handle(other.m_handle)
    {
      reference();
    }

    PhysicsHandle(PhysicsHandle&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr))
    {
    }

    ~PhysicsHandle()
    {
      if (m_handle == nullptr) {
        return;
      }

      unreference();
    }

    PhysicsHandle& operator=(const PhysicsHandle& other)
    {
      if (this != &other) {
        unreference();
        m_handle = other.m_handle;
        reference();
      }

      return *this;
    }

    PhysicsHandle& operator=(PhysicsHandle&& other) noexcept
    {
      if (this != &other) {
        unreference();
        m_handle = std::exchange(other.m_handle, nullptr);
      }

      return *this;
    }

    operator T*()
    {
      return m_handle;
    }

    operator const T *() const
    {
      return m_handle;
    }

    CounterType* user_data()
    {
      return static_cast<CounterType*>(UserDataGetter(m_handle));
    }

    void reference()
    {
      auto* counter = user_data();
      assert(counter != nullptr);
      ++(*counter);
    }

    void unreference()
    {
      auto* counter = user_data();
      assert(counter != nullptr);
      --(*counter);

      if (*counter == 0) {
        delete counter;
      }

      Destructor(m_handle);
      m_handle = nullptr;
    }

  private:
    T* m_handle = nullptr;
  };

}

#endif // GF_PHYSICS_HANDLE_H
