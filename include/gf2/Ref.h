// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_REF_H
#define GF_REF_H

#include <memory>
#include <type_traits>

namespace gf {

  template<typename T>
  class Ref {
  public:
    explicit Ref(T& ref) noexcept
    : m_ptr(std::addressof(ref))
    {
    }

    Ref([[maybe_unused]] T&& other) = delete; // NOLINT

    ~Ref() = default;

    Ref([[maybe_unused]] const Ref& other) noexcept = default;

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    Ref(const Ref<U>& other) noexcept
    : m_ptr(std::addressof(other.get()))
    {
    }

    Ref([[maybe_unused]] Ref&& other) = delete;

    Ref& operator=([[maybe_unused]] const Ref& other) noexcept = default;

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    Ref& operator=(const Ref<U>& other) noexcept
    {
      m_ptr = std::addressof(other.get());
    }

    Ref& operator=([[maybe_unused]] Ref&& other) = delete;

    T& get() const noexcept
    {
      return *m_ptr;
    }

    operator T&() const noexcept
    {
      return *m_ptr;
    }

  private:
    T* m_ptr;
  };

  template<typename T>
  constexpr bool operator==(Ref<T> lhs, Ref<T> rhs) noexcept
  {
    return std::addressof(lhs.get()) == std::addressof(rhs.get());
  }

  template<typename T>
  constexpr bool operator==(Ref<T> lhs, const T& rhs) noexcept
  {
    return std::addressof(lhs.get()) == std::addressof(rhs);
  }

  template<typename T>
  constexpr bool operator==(const T& lhs, Ref<T> rhs) noexcept
  {
    return std::addressof(lhs) == std::addressof(rhs.get());
  }

  template<typename T>
  Ref<T> ref(T& object)
  {
    return Ref<T>(object);
  }

  template<typename T>
  Ref<T> ref(const T&&) = delete;

} // namespace gf

#endif // GF_REF_H
