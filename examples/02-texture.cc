// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <array>

#include <gf2/core/Mat3.h>
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
  const std::filesystem::path texture_file = assets_directory / "root_company_game_over.jpg";

  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  const gf::GraphicsInitializer graphics;

  gf::Window window("02-texture | gf2", WindowSize);
  gf::Renderer renderer(&window);

  renderer.begin_memory_command_buffer();

  const gf::Vertex vertices[] = {
    {{ +0.5f, -0.5f }, { 1.0f, 0.0f }},
    {{ +0.5f, +0.5f }, { 1.0f, 1.0f }},
    {{ -0.5f, -0.5f }, { 0.0f, 0.0f }},
    {{ -0.5f, +0.5f }, { 0.0f, 1.0f }},
  };

  const gf::Buffer vertex_buffer(gf::BufferType::Device, gf::BufferUsage::Vertex, std::begin(vertices), std::size(vertices), &renderer);

  // clang-format off
  const uint16_t indices[] = {
    0, 1, 2, // first triangle
    2, 1, 3, // second triangle
  };
  // clang-format on

  const gf::Buffer index_buffer(gf::BufferType::Device, gf::BufferUsage::Index, std::begin(indices), std::size(indices), &renderer);

  const gf::Texture texture(texture_file, &renderer);

  renderer.end_memory_command_buffer();

  const gf::Mat3F identity = gf::Identity3F;

  while (!window.should_close()) {
    while (auto event = gf::Event::poll()) {
      switch (event->type) { // NOLINT(bugprone-unchecked-optional-access)
        case gf::EventType::Quit:
          window.close();
          break;

        case gf::EventType::WindowResized:
          renderer.recreate_swapchain();
          break;

        default:
          // do nothing
          break;
      }
    }

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

      auto descriptor = renderer.allocate_descriptor_for_pipeline(pipeline);
      descriptor.write(0, texture);
      render_command_buffer.bind_descriptor(pipeline, descriptor);

      render_command_buffer.push_constant(pipeline, gf::ShaderStage::Vertex, &identity);

      render_command_buffer.bind_vertex_buffer(&vertex_buffer);
      render_command_buffer.bind_index_buffer(&index_buffer);

      render_command_buffer.draw_indexed(std::size(indices));

      command_buffer.end_rendering(render_command_buffer);
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
