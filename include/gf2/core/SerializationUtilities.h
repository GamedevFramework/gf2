// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SERIALIZATION_UTILITIES_H
#define GF_SERIALIZATION_UTILITIES_H

#include <optional>
#include <tuple>
#include <variant>

#include "Serialization.h"
#include "SerializationOps.h"

namespace gf {

  /*
   * Supported types:
   * - std::monostate
   * - std::optional
   * - std::tuple
   * - std::variant
   */

  /*
   * Serializer
   */

  template<typename... Types>
  inline Serializer& operator|(Serializer& ar, const std::tuple<Types...>& tuple)
  {
    std::apply([&ar](auto&&... args) { ((ar | args), ...); }, tuple);
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

  /*
   * Deserializer
   */

  template<typename... Types>
  inline Deserializer& operator|(Deserializer& ar, std::tuple<Types...>& tuple)
  {
    std::apply([&ar](auto&&... args) { ((ar | args), ...); }, tuple);
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

}


#endif // GF_SERIALIZATION_UTILITIES_H
