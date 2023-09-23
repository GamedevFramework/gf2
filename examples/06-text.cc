// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <array>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/Font.h>
#include <gf2/core/FontManager.h>
#include <gf2/core/Mat3.h>
#include <gf2/core/Transform.h>

#include <gf2/graphics/Event.h>
#include <gf2/graphics/FontAtlas.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Text.h>
#include <gf2/graphics/Vertex.h>
#include <gf2/graphics/Window.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path font_file = assets_directory / "Oxanium-Regular.ttf";

  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  const gf::GraphicsInitializer graphics;
  gf::FontManager font_manager;

  gf::Window window("06-text | gf2", WindowSize);
  gf::Renderer renderer(&window);

  gf::Font font(font_file, &font_manager);
  gf::FontAtlas font_atlas(&font, gf::vec(2048, 2048), &renderer);

  gf::TextData text_data;
  text_data.content = "Gamedev Framework 2";
  text_data.character_size = 64;
  text_data.color = gf::darker(gf::Azure, 0.2f);

  const gf::Text text(&font_atlas, text_data, &renderer);

  gf::Camera camera;
  camera.type = gf::CameraType::Extend;
  camera.center = gf::vec(1000.0f, 1000.0f);
  camera.size = gf::vec(500.0f, 500.0f);

  camera.update(WindowSize);
  gf::Mat4F view_matrix(camera.compute_view_matrix());

  gf::Buffer camera_buffer(gf::BufferType::Device, gf::BufferUsage::Uniform, &view_matrix, 1, &renderer);

  gf::Transform transform;
  transform.location = gf::vec(1000.0f, 1000.0f);
  transform.origin = gf::vec(0.5f, 0.5f);

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

    if (auto maybe_command_buffer = renderer.begin_command_buffer(); maybe_command_buffer) {
      auto target = renderer.current_render_target();

      auto command_buffer = *maybe_command_buffer;
      auto render_command_buffer = command_buffer.begin_rendering(target, gf::White);

      const gf::RectF viewport = gf::RectF::from_size(target.extent());
      render_command_buffer.set_viewport(viewport);

      const gf::RectI scissor = gf::RectI::from_size(target.extent());
      render_command_buffer.set_scissor(scissor);

      const auto* pipeline = renderer.text_pipeline();

      render_command_buffer.bind_pipeline(pipeline);

      auto camera_descriptor = renderer.allocate_descriptor_for_layout(renderer.camera_descriptor());
      camera_descriptor.write(0, &camera_buffer);
      render_command_buffer.bind_descriptor(pipeline, 0, camera_descriptor);

      auto sampler_descriptor = renderer.allocate_descriptor_for_layout(renderer.sampler_descriptor());
      sampler_descriptor.write(0, text.texture());
      render_command_buffer.bind_descriptor(pipeline, 1, sampler_descriptor);

      const gf::Mat3F model_matrix = transform.compute_matrix(text.bounds());

      render_command_buffer.push_constant(pipeline, gf::ShaderStage::Vertex, &model_matrix);

      render_command_buffer.bind_vertex_buffer(text.vertices());
      render_command_buffer.bind_index_buffer(text.indices());

      render_command_buffer.draw_indexed(text.index_count());

      command_buffer.end_rendering(render_command_buffer);
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
