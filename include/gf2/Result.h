// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_RESULT_H
#define GF_RESULT_H

#include <cassert>

#include <type_traits>
#include <utility>
#include <variant>

namespace gf {

  template<typename E>
  struct Error {
    E error;
  };

  template<typename E>
  Error<E> error(E&& e)
  {
    return { std::forward<E>(e) };
  }


  template<typename T, typename E>
  class Result {
  public:
    Result() = default;

    template<typename U = T, typename X = std::enable_if_t<std::is_constructible_v<T, U>>>
    Result(U&& result)
    : m_result(std::in_place_index<0>, std::forward<U>(result))
    {
    }

    template<typename ... Args, typename X = std::enable_if_t<std::is_constructible_v<T, Args...>>>
    Result(std::in_place_t, Args&&... args)
    : m_result(std::in_place_index<0>, std::forward<Args>(args)...)
    {
    }

    Result(const Error<E>& result)
    : m_result(std::in_place_index<1>, result.error)
    {
    }

    Result(Error<E>&& result)
    : m_result(std::in_place_index<1>, std::move(result.error))
    {
    }

    Result(const Result&) = default;
    Result(Result&&) noexcept = default;
    ~Result() = default;

    Result& operator=(const Result&) = default;
    Result& operator=(Result&&) noexcept = default;

    template<typename U = T, typename X = std::enable_if_t<std::is_constructible_v<T, U>>>
    Result& operator=(U&& other)
    {
      m_result.template emplace<0>(std::forward<U>(other));
      return *this;
    }

    Result& operator=(const Error<E>& other)
    {
      m_result.template emplace<1>(other.error);
      return *this;
    }

    Result& operator=(Error<E>&& other)
    {
      m_result.template emplace<1>(std::move(other.error));
      return *this;
    }

    constexpr bool has_value() const noexcept
    {
      return m_result.index() == 0;
    }

    constexpr explicit operator bool() const noexcept
    {
      return m_result.index() == 0;
    }

    T& value() &
    {
      assert(m_result.index() == 0);
      return std::get<0>(m_result);
    }

    const T& value() const &
    {
      assert(m_result.index() == 0);
      return std::get<0>(m_result);
    }

    T&& value() &&
    {
      assert(m_result.index() == 0);
      return std::get<0>(std::move(m_result));
    }

    const T&& value() const &&
    {
      assert(m_result.index() == 0);
      return std::get<0>(std::move(m_result));
    }

    E& error() &
    {
      assert(m_result.index() == 1);
      return std::get<1>(m_result);
    }

    const E& error() const &
    {
      assert(m_result.index() == 1);
      return std::get<1>(m_result);
    }

    E&& error() &&
    {
      assert(m_result.index() == 1);
      return std::get<1>(std::move(m_result));
    }

    const E&& error() const &&
    {
      assert(m_result.index() == 1);
      return std::get<1>(std::move(m_result));
    }

    T* operator->() noexcept
    {
      assert(m_result.index() == 0);
      return &std::get<0>(m_result);
    }

    const T* operator->() const noexcept
    {
      assert(m_result.index() == 0);
      return &std::get<0>(m_result);
    }

    T& operator*() &
    {
      return value();
    }

    const T& operator*() const &
    {
      return value();
    }

    T&& operator*() &&
    {
      return value();
    }

    const T&& operator*() const &&
    {
      return value();
    }

    template<typename U>
    constexpr T value_or(U&& default_value) const &
    {
      if (has_value()) {
        return std::get<0>(m_result);
      }

      return static_cast<T>(std::forward<U>(default_value));
    }

    template<typename U>
    constexpr T value_or(U&& default_value) &&
    {
      if (has_value()) {
        return std::get<0>(std::move(m_result));
      }

      return static_cast<T>(std::forward<U>(default_value));
    }

  private:
    std::variant<T, E> m_result;
  };

}

#endif // GF_RESULT_H
