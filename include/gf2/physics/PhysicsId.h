// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_ID_H
#define GF_PHYSICS_ID_H

#include <cstdint>

#include <array>

#include <box2d/box2d.h>

namespace gf {

  enum class PhysicsWorldId : uint32_t;
  enum class PhysicsBodyId : uint64_t;
  enum class PhysicsShapeId : uint64_t;
  enum class PhysicsChainId : uint64_t;
  enum class PhysicsJointId : uint64_t;
  using PhysicsContactId = std::array<uint32_t, 3>;

  namespace details {

    inline b2WorldId to_raw(PhysicsWorldId id)
    {
      return b2LoadWorldId(static_cast<uint32_t>(id));
    }

    inline PhysicsWorldId to_id(b2WorldId raw)
    {
      return static_cast<PhysicsWorldId>(b2StoreWorldId(raw));
    }

    inline b2BodyId to_raw(PhysicsBodyId id)
    {
      return b2LoadBodyId(static_cast<uint64_t>(id));
    }

    inline PhysicsBodyId to_id(b2BodyId raw)
    {
      return static_cast<PhysicsBodyId>(b2StoreBodyId(raw));
    }

    inline b2ShapeId to_raw(PhysicsShapeId id)
    {
      return b2LoadShapeId(static_cast<uint64_t>(id));
    }

    inline PhysicsShapeId to_id(b2ShapeId raw)
    {
      return static_cast<PhysicsShapeId>(b2StoreShapeId(raw));
    }

    inline b2ChainId to_raw(PhysicsChainId id)
    {
      return b2LoadChainId(static_cast<uint64_t>(id));
    }

    inline PhysicsChainId to_id(b2ChainId raw)
    {
      return static_cast<PhysicsChainId>(b2StoreChainId(raw));
    }

    inline b2JointId to_raw(PhysicsJointId id)
    {
      return b2LoadJointId(static_cast<uint64_t>(id));
    }

    inline PhysicsJointId to_id(b2JointId raw)
    {
      return static_cast<PhysicsJointId>(b2StoreJointId(raw));
    }

    inline b2ContactId to_raw(PhysicsContactId id)
    {
      return b2LoadContactId(id.data());
    }

    inline PhysicsContactId to_id(b2ContactId raw)
    {
      PhysicsContactId id = {};
      b2StoreContactId(raw, id.data());
      return id;
    }


  }

}

#endif // GF_PHYSICS_ID_H
