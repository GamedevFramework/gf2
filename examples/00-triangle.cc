// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <array>

#include <gf2/core/Mat3.h>
#include <gf2/core/Mat4.h>

#include <gf2/graphics/Event.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Vertex.h>
#include <gf2/graphics/Window.h>

int main()
{
  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  const gf::GraphicsInitializer graphics;

  gf::Window window("00-triangle | gf2", WindowSize);
  gf::Renderer renderer(&window);

  const gf::SimpleVertex vertices[] = {
    { { +0.0f, -0.5f },       gf::Rose },
    { { +0.5f, +0.5f }, gf::Chartreuse },
    { { -0.5f, +0.5f },      gf::Azure },
  };

  const gf::Buffer buffer(gf::BufferType::Device, gf::BufferUsage::Vertex, std::begin(vertices), std::size(vertices), &renderer);

  const gf::Mat3F identity = gf::Identity3F;
  const gf::Mat4F aligned_identity(identity);
  const gf::Buffer camera_buffer(gf::BufferType::Device, gf::BufferUsage::Uniform, &aligned_identity, 1, &renderer);

  while (!window.should_close()) {
    while (auto event = gf::Event::poll()) {
      switch (event->type) {
        case gf::EventType::Quit:
          window.close();
          break;

        case gf::EventType::WindowResized:
          renderer.update_surface_size(window.surface_size());
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

      const auto* pipeline = renderer.simple_pipeline();

      auto descriptor = renderer.allocate_descriptor_for_layout(renderer.camera_descriptor());
      descriptor.write(0, &camera_buffer);
      render_command_buffer.bind_descriptor(pipeline, 0, descriptor);

      render_command_buffer.bind_pipeline(pipeline);

      render_command_buffer.push_constant(pipeline, gf::ShaderStage::Vertex, &identity);

      render_command_buffer.bind_vertex_buffer(&buffer);

      render_command_buffer.draw(std::size(vertices));

      command_buffer.end_rendering(render_command_buffer);
      renderer.end_command_buffer(command_buffer);
    }
  }

  renderer.wait_idle();
}
