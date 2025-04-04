// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SERIALIZATION_ADAPTER_H
#define GF_SERIALIZATION_ADAPTER_H

#include <queue>

#include "Serialization.h"

namespace gf {

  /*
   * Supported types:
   * - std::priority_queue
   * - std::queue
   */

  namespace details {

    template<typename Adapter>
    decltype(auto) underlying_container(Adapter& adapter)
    {
      struct Hack : Adapter {
        static decltype(auto) get(Adapter& adapter)
        {
          return adapter.*&Hack::c;
        }
      };

      return Hack::get(adapter);
    }

  }

  /*
   * Serializer
   */

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::queue<T>& queue)
  {
    return ar | details::underlying_container(queue);
  }

  template<typename T>
  inline Serializer& operator|(Serializer& ar, const std::priority_queue<T>& queue)
  {
    return ar | details::underlying_container(queue);
  }

  /*
   * Deserializer
   */

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::queue<T>& queue)
  {
    details::underlying_container(queue).clear();
    return ar | details::underlying_container(queue);
  }

  template<typename T>
  inline Deserializer& operator|(Deserializer& ar, std::priority_queue<T>& queue)
  {
    details::underlying_container(queue).clear();
    return ar | details::underlying_container(queue);
  }

}

#endif // GF_SERIALIZATION_ADAPTER_H
