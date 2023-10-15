// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SERIALIZATION_OPS_H
#define GF_SERIALIZATION_OPS_H

#include <cstdint>
#include <cstring>

#include <array>
#include <filesystem>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "Array2D.h"
#include "Flags.h"
#include "Serialization.h"
#include "Span.h"

namespace gf {

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

  inline Serializer& operator|(Serializer& ar, bool data)
  {
    ar.write_bool(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, char data)
  {
    ar.write_char(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint8_t data)
  {
    ar.write_u8(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint16_t data)
  {
    ar.write_u16(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint32_t data)
  {
    ar.write_u32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, uint64_t data)
  {
    ar.write_u64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int8_t data)
  {
    ar.write_i8(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int16_t data)
  {
    ar.write_i16(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int32_t data)
  {
    ar.write_i32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, int64_t data)
  {
    ar.write_i64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, float data)
  {
    ar.write_f32(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, double data)
  {
    ar.write_f64(data);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, const std::string& str)
  {
    ar.write_raw_string(str.data(), str.length());
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, const std::filesystem::path& path)
  {
    return ar | path.string();
  }

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  inline Serializer& operator|(Serializer& ar, T data)
  {
    using U = std::underlying_type_t<T>;
    return ar | static_cast<U>(data);
  }

  template<typename E>
  inline Serializer& operator|(Serializer& ar, Flags<E> data)
  {
    return ar | data.m_data;
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const Array2D<T>& array)
  {
    const Vec2I size = array.size();
    ar | size.w | size.h;

    for (auto& item : array) {
      ar | item;
    }

    return ar;
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
  inline Serializer& operator|(Serializer& ar, const std::set<T>& set)
  {
    return details::write_container(ar, set.begin(), set.size());
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::unordered_set<T>& set)
  {
    return details::write_container(ar, set.begin(), set.size());
  }

  template<typename K, typename V>
  inline Serializer& operator|(Serializer& ar, const std::pair<K, V>& pair)
  {
    return ar | pair.first | pair.second;
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

  template<typename... Types>
  inline Serializer& operator|(Serializer& ar, const std::tuple<Types...>& tuple)
  {
    std::apply([&ar](auto&&... args) { ((ar | args), ...);  }, tuple);
    return ar;
  }

  inline Serializer& operator|(Serializer& ar, const std::monostate& /* unused */)
  {
    return ar;
  }

  namespace details {

    template<typename T, typename... Types>
    void variant_serializer(Serializer& ar, const std::variant<Types...>& variant)
    {
      ar | std::get<T>(variant);
    }

  }

  template<typename... Types>
  inline Serializer& operator|(Serializer& ar, const std::variant<Types...>& variant)
  {
    std::size_t index = variant.index();
    ar.write_raw_size(index);

    using VariantSerializer = void (*)(Serializer& ar, const std::variant<Types...>& variant);

    VariantSerializer serializers[] = {
      &details::variant_serializer<Types, Types...>...,
    };

    serializers[index](ar, variant);

    return ar;
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::optional<T>& optional)
  {
    if (optional.has_value()) {
      ar | true | optional.value();
    } else {
      ar | false;
    }

    return ar;
  }

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

  inline Deserializer& operator|(Deserializer& ar, bool& data)
  {
    ar.read_bool(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, char& data)
  {
    ar.read_char(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint8_t& data)
  {
    ar.read_u8(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint16_t& data)
  {
    ar.read_u16(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint32_t& data)
  {
    ar.read_u32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, uint64_t& data)
  {
    ar.read_u64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int8_t& data)
  {
    ar.read_i8(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int16_t& data)
  {
    ar.read_i16(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int32_t& data)
  {
    ar.read_i32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, int64_t& data)
  {
    ar.read_i64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, float& data)
  {
    ar.read_f32(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, double& data)
  {
    ar.read_f64(&data);
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, std::string& str)
  {
    std::size_t size = 0;
    ar.read_raw_size(&size);
    str.resize(size);
    ar.read_raw_string(str.data(), str.size());
    return ar;
  }

  inline Deserializer& operator|(Deserializer& ar, std::filesystem::path& path)
  {
    std::string str;
    ar | str;
    path = std::move(str);
    return ar;
  }

  template<typename T, typename E = std::enable_if_t<std::is_enum_v<T>, T>>
  inline Deserializer& operator|(Deserializer& ar, T& data)
  {
    using U = std::underlying_type_t<T>;
    U underlying;
    ar | underlying;
    data = static_cast<T>(underlying);
    return ar;
  }

  template<typename E>
  inline Deserializer& operator|(Deserializer& ar, Flags<E>& data)
  {
    return ar | data.m_data;
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, Array2D<T>& array)
  {
    Vec2I size(0, 0);
    ar | size.w | size.h;

    Array2D<T> tmp(size);

    for (auto& item : tmp) {
      ar | item;
    }

    array = std::move(tmp);
    return ar;
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

  template<typename T, std::size_t N>
  inline Deserializer& operator|(Deserializer& ar, std::array<T, N>& array)
  {
    return details::read_container<T>(ar, array.begin(), array.size());
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::vector<T>& array)
  {
    array.clear();
    return details::read_container<T>(ar, std::back_inserter(array));
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

  template<typename K, typename V>
  inline Deserializer& operator|(Deserializer& ar, std::pair<K, V>& pair)
  {
    return ar | pair.first | pair.second;
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

  template<typename... Types>
  inline Deserializer& operator|(Deserializer& ar, std::tuple<Types...>& tuple)
  {
    std::apply([&ar](auto&&... args) { ((ar | args), ...);  }, tuple);
    return ar;
  }

  namespace details {

    template<typename T, typename... Types>
    std::variant<Types...> variant_deserializer(Deserializer& ar)
    {
      T value;
      ar | value;
      return value;
    }

  }

  inline Deserializer& operator|(Deserializer& ar, std::monostate& /* unused */)
  {
    return ar;
  }

  template<typename... Types>
  inline Deserializer& operator|(Deserializer& ar, std::variant<Types...>& variant)
  {
    std::size_t index = 0;
    ar.read_raw_size(&index);

    if (index >= sizeof...(Types)) {
      throw std::out_of_range("Variant index out of range");
    }

    using VariantDeserializerType = std::variant<Types...> (*)(Deserializer& ar);

    VariantDeserializerType deserializers[] = {
      &details::variant_deserializer<Types, Types...>...,
    };

    variant = deserializers[index](ar);
    return ar;
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::optional<T>& optional)
  {
    bool has_value = false;
    ar | has_value;

    if (has_value) {
      T value;
      ar | value;
      optional = std::move(value);
    } else {
      optional = std::nullopt;
    }

    return ar;
  }

} // namespace gf

#endif // GF_SERIALIZATION_OPS_H
