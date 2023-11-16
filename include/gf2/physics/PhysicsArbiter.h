// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_ARBITER_H
#define GF_PHYSICS_ARBITER_H

struct cpArbiter;

namespace gf {

  class PhysicsArbiter {
  public:
    PhysicsArbiter(const PhysicsArbiter&) = delete;
    PhysicsArbiter(PhysicsArbiter&&) noexcept = delete;
    ~PhysicsArbiter() = default;

    PhysicsArbiter& operator=(const PhysicsArbiter&) = delete;
    PhysicsArbiter& operator=(PhysicsArbiter&&) noexcept = delete;

  private:
    friend class PhysicsBody;

    PhysicsArbiter(cpArbiter* arbiter);

    cpArbiter* m_arbiter = nullptr;
  };

}

#endif // GF_PHYSICS_ARBITER_H
