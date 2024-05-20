#include <gf2/graphics/SceneManager.h>

// tag::class[]
class SuperRPG3000 : public gf::SceneManager {
public:
  SuperRPG3000();

private:
  // put scenes of the game here
};
// end::class[]

// tag::constructor[]
SuperRPG3000::SuperRPG3000()
: gf::SceneManager("Super RPG 3000", { 1920, 1080 }, ~gf::WindowHints::Resizable)
{
  // initialize the scenes of the game
  // push one or more scenes
}
// end::constructor[]

