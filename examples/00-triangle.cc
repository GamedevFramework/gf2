// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/Event.h>
#include <gf2/GraphicsInitializer.h>
#include <gf2/Renderer.h>
#include <gf2/Vertex.h>
#include <gf2/Window.h>

int main()
{
  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  gf::GraphicsInitializer graphics;

  gf::Window window("00-triangle | gf2", WindowSize);
  gf::Renderer renderer(&window);

  const gf::SimpleVertex vertices[] = {
    {{ +0.0f, -0.5f },       gf::Rose},
    {{ +0.5f, +0.5f }, gf::Chartreuse},
    {{ -0.5f, +0.5f },      gf::Azure},
  };

  gf::Buffer buffer = renderer.allocate_buffer(gf::BufferType::Device, gf::BufferUsage::Vertex, vertices, std::size(vertices));

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
      command_buffer.bind_vertex_buffer(&buffer);

      command_buffer.draw(std::size(vertices));

      command_buffer.end_rendering();
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
