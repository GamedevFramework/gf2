// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsChain.h>

#include <cassert>

#include <utility>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  PhysicsChain::PhysicsChain(PhysicsBody* body, const PhysicsChainData& data)
  : m_owner(details::PhysicsOwner::Object)
  {
    assert(body != nullptr);
    assert(body->valid());

    std::vector<b2Vec2> raw_points;
    raw_points.reserve(data.points.size());

    for (const Vec2F point : data.points) {
      raw_points.push_back({ point.x, point.y });
    }

    const b2SurfaceMaterial raw_material = details::to_raw(data.material);

    b2ChainDef def = b2DefaultChainDef();
    def.points = raw_points.data();
    def.count = static_cast<int>(raw_points.size());
    def.materials = &raw_material;
    def.materialCount = 1;
    def.filter = details::to_raw(data.filter);
    def.isLoop = data.loop;
    def.enableSensorEvents = data.enable_sensor_events;
    def.userData = data.user_data;

    m_id = b2CreateChain(details::to_raw(body->id()), &def);
  }

  PhysicsChain::PhysicsChain(PhysicsChain&& other) noexcept
  : m_owner(std::exchange(other.m_owner, details::PhysicsOwner::None))
  , m_id(std::exchange(other.m_id, b2_nullChainId))
  {

  }

  PhysicsChain::~PhysicsChain()
  {
    if (m_owner == details::PhysicsOwner::Object) {
      b2DestroyChain(m_id);
    }
  }

  PhysicsChain& PhysicsChain::operator=(PhysicsChain&& other) noexcept
  {
    std::swap(other.m_owner, m_owner);
    std::swap(other.m_id, m_id);
    return *this;
  }

  PhysicsChainId PhysicsChain::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsChain PhysicsChain::from_id(PhysicsChainId id)
  {
    return { details::PhysicsOwner::None, details::to_raw(id) };
  }

  PhysicsWorld PhysicsChain::world() const
  {
    return PhysicsWorld::from_id(details::to_id(b2Chain_GetWorld(m_id)));
  }

  bool PhysicsChain::valid() const
  {
    return b2Chain_IsValid(m_id);
  }

  std::vector<PhysicsShape> PhysicsChain::segments() const
  {
    const int count = b2Chain_GetSegmentCount(m_id);

    std::vector<b2ShapeId> raw_shapes(count);
    [[maybe_unused]] const int actual_count = b2Chain_GetSegments(m_id, raw_shapes.data(), count);
    assert(count == actual_count);

    std::vector<PhysicsShape> shapes;
    shapes.reserve(count);

    for (const b2ShapeId raw : raw_shapes) {
      shapes.push_back(PhysicsShape::from_id(details::to_id(raw)));
    }

    return shapes;
  }

  PhysicsSurfaceMaterial PhysicsChain::surface_material() const
  {
    return details::to_material(b2Chain_GetSurfaceMaterial(m_id, 0));
  }

  void PhysicsChain::set_surface_materiel(const PhysicsSurfaceMaterial& material)
  {
    const b2SurfaceMaterial raw = details::to_raw(material);
    b2Chain_SetSurfaceMaterial(m_id, &raw, 0);
  }

}
