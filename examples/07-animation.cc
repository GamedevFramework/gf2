// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <array>

#include <gf2/core/AnimationData.h>
#include <gf2/core/Camera.h>
#include <gf2/core/Clock.h>
#include <gf2/core/Mat3.h>
#include <gf2/core/Transform.h>

#include <gf2/graphics/AnimatedSprite.h>
#include <gf2/graphics/Event.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>
#include <gf2/graphics/Window.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path texture_file0 = assets_directory / "splash_screen_0.png";
  const std::filesystem::path texture_file1 = assets_directory / "splash_screen_1.png";
  const std::filesystem::path texture_file2 = assets_directory / "splash_screen_2.png";
  const std::filesystem::path texture_file3 = assets_directory / "splash_screen_3.png";
  const std::filesystem::path texture_file4 = assets_directory / "splash_screen_4.png";
  const std::filesystem::path texture_file5 = assets_directory / "splash_screen_5.png";
  const std::filesystem::path texture_file6 = assets_directory / "splash_screen_6.png";
  const std::filesystem::path texture_file7 = assets_directory / "splash_screen_7.png";

  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  const gf::GraphicsInitializer graphics;

  gf::Window window("07-animation | gf2", WindowSize);
  gf::Renderer renderer(&window);

  const gf::Texture texture0(texture_file0, &renderer);
  const gf::Texture texture1(texture_file1, &renderer);
  const gf::Texture texture2(texture_file2, &renderer);
  const gf::Texture texture3(texture_file3, &renderer);
  const gf::Texture texture4(texture_file4, &renderer);
  const gf::Texture texture5(texture_file5, &renderer);
  const gf::Texture texture6(texture_file6, &renderer);
  const gf::Texture texture7(texture_file7, &renderer);

  const std::vector<const gf::Texture*> textures = { &texture0, &texture1, &texture2, &texture3, &texture4, &texture5, &texture6, &texture7 };

  gf::AnimationData data = {};
  data.properties = gf::AnimationProperties::Loop;

  for (uint32_t i = 0; i < 7; ++i) {
    data.add_tileset(i, { 6, 4 }, gf::seconds(1.0f / 25.0f), 24);
  }

  data.add_tileset(7, { 6, 4 }, gf::seconds(1.0f / 25.0f), 4);

  gf::AnimatedSprite sprite(textures, data, &renderer);

  gf::Camera camera;
  camera.type = gf::CameraType::Extend;
  camera.center = gf::vec(1000.0f, 1000.0f);
  camera.size = gf::vec(50.0f, 100.0f);

  camera.update(WindowSize);
  gf::Mat4F view_matrix(camera.compute_view_matrix());

  gf::Buffer camera_buffer(gf::BufferType::Device, gf::BufferUsage::Uniform, &view_matrix, 1, &renderer);

  gf::Transform transform;
  transform.location = gf::vec(1000.0f, 1000.0f);
  transform.origin = gf::vec(0.5f, 0.5f);
  transform.scale = gf::vec(0.1f, 0.1f);

  gf::Clock clock;

  while (!window.should_close()) {
    while (auto event = gf::Event::poll()) {
      switch (event->type) {
        case gf::EventType::Quit:
          window.close();
          break;

        case gf::EventType::WindowResized:
          renderer.update_surface_size(window.surface_size());
          camera.update(window.surface_size());
          view_matrix = camera.compute_view_matrix();
          camera_buffer.update(&view_matrix, 1, &renderer);
          break;

        default:
          // do nothing
          break;
      }
    }

    const gf::Time time = clock.restart();
    sprite.update(time);

    if (auto maybe_command_buffer = renderer.begin_command_buffer(); maybe_command_buffer) {
      auto target = renderer.current_render_target();

      auto command_buffer = *maybe_command_buffer;
      auto render_command_buffer = command_buffer.begin_rendering(target);

      const gf::RectF viewport = gf::RectF::from_size(target.extent());
      render_command_buffer.set_viewport(viewport);

      const gf::RectI scissor = gf::RectI::from_size(target.extent());
      render_command_buffer.set_scissor(scissor);

      const auto* pipeline = renderer.default_pipeline();

      render_command_buffer.bind_pipeline(pipeline);

      auto camera_descriptor = renderer.allocate_descriptor_for_layout(renderer.camera_descriptor());
      camera_descriptor.write(0, &camera_buffer);
      render_command_buffer.bind_descriptor(pipeline, 0, camera_descriptor);

      auto sampler_descriptor = renderer.allocate_descriptor_for_layout(renderer.sampler_descriptor());
      sampler_descriptor.write(0, sprite.texture());
      render_command_buffer.bind_descriptor(pipeline, 1, sampler_descriptor);

      const gf::Mat3F model_matrix = transform.compute_matrix(sprite.bounds());

      render_command_buffer.push_constant(pipeline, gf::ShaderStage::Vertex, &model_matrix);

      render_command_buffer.bind_vertex_buffer(sprite.vertices());
      render_command_buffer.bind_index_buffer(sprite.indices());

      render_command_buffer.draw_indexed(sprite.index_count(), sprite.first_index());

      command_buffer.end_rendering(render_command_buffer);
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
