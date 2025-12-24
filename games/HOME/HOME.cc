
#include "config.h"

#include <gf2/physics/PhysicsWorld.h>

#include "bits/GameHub.h"

int main()
{
  gf::set_physics_length_units_per_meter(20.0f);

  home::GameHub hub(GF_HOME_ASSETS_DIRECTORY);
  return hub.run();
}
