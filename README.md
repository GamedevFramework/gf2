![Gamedev Framework 2](docs/assets/logoex.png)

# Gamedev Framework 2

**Gamedev Framework 2** (gf2) is a modern framework to build 2D games in C++17. It is based on [SDL 3](https://libsdl.org/) and [Vulkan 1.3](https://www.vulkan.org/) and provides high-level constructions to easily build games.

> [!WARNING]
> This project is in **alpha stage**. The API is not stabilized and may change during the development.
> You can use [Gamedev Framework 1](https://github.com/GamedevFramework/gf).

## Example

The following example illustrates how to draw a simple sprite.

```cc
#include <cstdlib>

#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/SpriteEntity.h>

int main()
{
  gf::SingleSceneManager scene_manager("my game", gf::vec(1600, 900));

  const gf::Texture texture("logo.png", scene_manager.renderer());
  gf::SpriteEntity sprite_entity(&texture, scene_manager.renderer());
  sprite_entity.transform().location = gf::vec(100.0f, 100.0f);
  sprite_entity.transform().origin = gf::vec(0.5f, 0.5f);

  gf::StandardScene scene;
  scene.set_world_center({ 100.0f, 100.0f });
  scene.set_world_size({ 200.0f, 200.0f });

  scene.add_world_entity(&sprite_entity);

  scene_manager.run(&scene);

  return EXIT_SUCCESS;
}
```


## License

Gamedev Framework 2 is licensed under the terms and conditions of the [zlib/libpng license](https://opensource.org/licenses/Zlib).
