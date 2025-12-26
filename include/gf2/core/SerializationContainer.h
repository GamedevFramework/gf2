// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SERIALIZATION_CONTAINER_H
#define GF_SERIALIZATION_CONTAINER_H

#include <array>
#include <deque>
#include <iterator>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Serialization.h"

namespace gf {

  /*
   * Supported types:
   * - std::array
   * - std::deque
   * - std::map
   * - std::set
   * - std::unordered_map
   * - std::unordered_set
   * - std::vector
   */

  /*
   * Serializer
   */

  namespace details {

    template<typename Iterator>
    Serializer& write_container(Serializer& ar, Iterator iterator, std::size_t size)
    {
      ar.write_raw_size(size);

      for (std::size_t i = 0; i < size; ++i) {
        ar | *iterator++;
      }

      return ar;
    }

  }

  template<typename T, std::size_t N>
  inline Serializer& operator|(Serializer& ar, const T (&array)[N])
  {
    return details::write_container(ar, std::begin(array), std::size(array));
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::vector<T>& array)
  {
    return details::write_container(ar, array.begin(), array.size());
  }

  template<typename T, std::size_t N>
  inline Serializer& operator|(Serializer& ar, const std::array<T, N>& array)
  {
    return details::write_container(ar, array.begin(), array.size());
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::deque<T>& deque)
  {
    return details::write_container(ar, deque.begin(), deque.size());
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::set<T>& set)
  {
    return details::write_container(ar, set.begin(), set.size());
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::unordered_set<T>& set)
  {
    return details::write_container(ar, set.begin(), set.size());
  }

  template<typename K, typename V, typename C>
  inline Serializer& operator|(Serializer& ar, const std::map<K, V, C>& map)
  {
    return details::write_container(ar, map.begin(), map.size());
  }

  template<typename K, typename V>
  inline Serializer& operator|(Serializer& ar, const std::unordered_map<K, V>& map)
  {
    return details::write_container(ar, map.begin(), map.size());
  }

  /*
   * Deserializer
   */

  namespace details {

    template<typename Container>
    class EmplaceIterator {
    public:
      using iterator_category = std::output_iterator_tag;
      using value_type = void;
      using difference_type = void;
      using pointer = void;
      using reference = void;
      using container_type = Container;

      explicit EmplaceIterator(Container& container)
      : m_container(std::addressof(container))
      {
      }

      template<typename T>
      EmplaceIterator& operator=(T&& value)
      {
        m_container->emplace(std::forward<T>(value));
        return *this;
      }

      EmplaceIterator& operator*() { return *this; }
      EmplaceIterator& operator++() { return *this; }
      EmplaceIterator operator++(int) { return *this; }

    private:
      Container* m_container;
    };

    template<typename Container>
    EmplaceIterator<Container> emplacer(Container& container)
    {
      return EmplaceIterator<Container>(container);
    }

    template<typename T, typename Iterator>
    Deserializer& read_container(Deserializer& ar, Iterator iterator, std::size_t expected_size = 0)
    {
      std::size_t size = 0;
      ar.read_raw_size(&size);

      if (expected_size > 0 && size != expected_size) {
        return ar;
      }

      for (std::size_t i = 0; i < size; ++i) {
        T item;
        ar | item;
        *iterator++ = std::move(item);
      }

      return ar;
    }

  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, Span<T> array)
  {
    return details::read_container<T>(ar, array.begin(), array.size());
  }

  template<typename T, std::size_t N>
  inline Deserializer& operator|(Deserializer& ar, T (&array)[N])
  {
    return details::read_container<T>(ar, std::begin(array), std::size(array));
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::vector<T>& array)
  {
    array.clear();
    return details::read_container<T>(ar, std::back_inserter(array));
  }

  template<typename T, std::size_t N>
  inline Deserializer& operator|(Deserializer& ar, std::array<T, N>& array)
  {
    return details::read_container<T>(ar, array.begin(), array.size());
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::deque<T>& deque)
  {
    deque.clear();
    return details::read_container<T>(ar, std::back_inserter(deque));
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::set<T>& set)
  {
    set.clear();
    return details::read_container<T>(ar, details::emplacer(set));
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::unordered_set<T>& set)
  {
    set.clear();
    return details::read_container<T>(ar, details::emplacer(set));
  }

  template<typename K, typename V, typename C>
  inline Deserializer& operator|(Deserializer& ar, std::map<K, V, C>& map)
  {
    map.clear();
    return details::read_container<std::pair<K, V>>(ar, details::emplacer(map));
  }

  template<typename K, typename V>
  inline Deserializer& operator|(Deserializer& ar, std::unordered_map<K, V>& map)
  {
    map.clear();
    return details::read_container<std::pair<K, V>>(ar, details::emplacer(map));
  }

}

#endif // GF_SERIALIZATION_CONTAINER_H
