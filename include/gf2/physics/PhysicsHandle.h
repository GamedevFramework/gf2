// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_HANDLE_H
#define GF_PHYSICS_HANDLE_H

#include <cassert>
#include <cstdint>

#include <utility>

#include "PhysicsId.h"

namespace gf::details {

  class PhysicsExistingType {
  };

  constexpr PhysicsExistingType PhysicsExisting = {};

  class PhysicsNoReferenceType {
  };

  constexpr PhysicsNoReferenceType PhysicsNoReference = {};

  template<typename T, void* (*UserDataGetter)(const T*), void (*UserDataSetter)(T*, void*), void (*Destructor)(T*)>
  class PhysicsHandle {
  public:
    PhysicsHandle()
    : m_handle(nullptr)
    {
    }

    PhysicsHandle(T* handle)
    : m_handle(handle)
    {
      set_refcount(1);
    }

    PhysicsHandle([[maybe_unused]] PhysicsExistingType existing, T* handle)
    : m_handle(handle)
    {
      reference();
    }

    PhysicsHandle([[maybe_unused]] PhysicsNoReferenceType no_reference, T* handle)
    : m_handle(handle)
    {
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
      std::swap(m_handle, other.m_handle);
      return *this;
    }

    PhysicsId id() const
    {
      return PhysicsId{ reinterpret_cast<uintptr_t>(m_handle) }; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    }

    T* get()
    {
      return m_handle;
    }

    const T* get() const
    {
      return m_handle;
    }

    operator T*()
    {
      return m_handle;
    }

    operator const T *() const
    {
      return m_handle;
    }

    uintptr_t refcount() const
    {
      assert(m_handle != nullptr);
      return reinterpret_cast<uintptr_t>(UserDataGetter(m_handle)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    }

    void set_refcount(uintptr_t count)
    {
      assert(m_handle != nullptr);
      UserDataSetter(m_handle, reinterpret_cast<void*>(count)); // NOLINT(performance-no-int-to-ptr,cppcoreguidelines-pro-type-reinterpret-cast)
    }

    void reference()
    {
      auto count = refcount();
      ++count;
      set_refcount(count);
    }

    void unreference()
    {
      auto count = refcount();
      --count;

      if (count == 0) {
        Destructor(m_handle);
        m_handle = nullptr;
      } else {
        set_refcount(count);
      }
    }

  private:
    T* m_handle = nullptr;
  };

}

#endif // GF_PHYSICS_HANDLE_H
