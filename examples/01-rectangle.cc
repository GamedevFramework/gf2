// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <array>

#include <gf2/Event.h>
#include <gf2/GraphicsInitializer.h>
#include <gf2/Renderer.h>
#include <gf2/Vertex.h>
#include <gf2/Window.h>

int main()
{
  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  gf::GraphicsInitializer graphics;

  gf::Window window("01-rectangle | gf2", WindowSize);
  gf::Renderer renderer(&window);

  renderer.begin_memory_command_buffer();

  const gf::SimpleVertex vertices[] = {
    {{ +0.5f, -0.5f },       gf::Rose},
    {{ +0.5f, +0.5f }, gf::Chartreuse},
    {{ -0.5f, -0.5f },      gf::Azure},
    {{ -0.5f, +0.5f },      gf::Yellow},
  };

  gf::Buffer vertex_buffer(gf::BufferType::Device, gf::BufferUsage::Vertex, vertices, std::size(vertices), &renderer);

  const uint16_t indices[] = {
    0, 1, 2, // first triangle
    2, 1, 3, // second triangle
  };

  gf::Buffer index_buffer(gf::BufferType::Device, gf::BufferUsage::Index, indices, std::size(indices), &renderer);

  renderer.end_memory_command_buffer();

  while (!window.closed()) {
    while (auto event = gf::Event::poll()) {
      switch (event->type) {
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
      command_buffer.begin_rendering(target);

      const gf::RectF viewport = gf::RectF::from_size(target.extent());
      command_buffer.set_viewport(viewport);

      const gf::RectI scissor = gf::RectI::from_size(target.extent());
      command_buffer.set_scissor(scissor);

      command_buffer.bind_pipeline(renderer.simple_pipeline());
      command_buffer.bind_vertex_buffer(&vertex_buffer);
      command_buffer.bind_index_buffer(&index_buffer);

      command_buffer.draw_indexed(std::size(indices));

      command_buffer.end_rendering();
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
