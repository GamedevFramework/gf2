// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/GpuGraphicsPipeline.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <utility>
#include <vector>

#include <gf2/core/Log.h>

#include <gf2/graphics/RenderManager.h>
#include "SDL3/SDL_gpu.h"

namespace gf {

  /*
   * RenderPipelineBuilder
   */

  GpuGraphicsPipeline GpuGraphicsPipelineBuilder::build(RenderManager* render_manager)
  {
    SDL_GPUGraphicsPipelineCreateInfo info = {};

    // shaders

    info.vertex_shader = m_vertex_shader->m_handle.object();
    info.fragment_shader = m_fragment_shader->m_handle.object();

    // vertex input

    std::vector<SDL_GPUVertexBufferDescription> vertex_buffer_descriptions;

    for (const GpuVertexBufferDescription& raw_vertex_buffer_description : m_vertex_input.buffer_descriptions) {
      SDL_GPUVertexBufferDescription vertex_buffer_description = {};
      vertex_buffer_description.slot = raw_vertex_buffer_description.slot;
      vertex_buffer_description.pitch = static_cast<Uint32>(raw_vertex_buffer_description.pitch);
      vertex_buffer_description.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
      vertex_buffer_description.instance_step_rate = 0;

      vertex_buffer_descriptions.push_back(vertex_buffer_description);
    }

    info.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions.data();
    info.vertex_input_state.num_vertex_attributes = static_cast<Uint32>(vertex_buffer_descriptions.size());

    std::vector<SDL_GPUVertexAttribute> vertex_attributes;

    for (const GpuVertexAttribute& raw_vertex_attribute : m_vertex_input.attributes) {
      SDL_GPUVertexAttribute vertex_attribute = {};
      vertex_attribute.location = raw_vertex_attribute.location;
      vertex_attribute.buffer_slot = raw_vertex_attribute.slot;
      vertex_attribute.format = static_cast<SDL_GPUVertexElementFormat>(raw_vertex_attribute.format);
      vertex_attribute.offset = static_cast<Uint32>(raw_vertex_attribute.offset);

      vertex_attributes.push_back(vertex_attribute);
    }

    info.vertex_input_state.vertex_attributes = vertex_attributes.data();
    info.vertex_input_state.num_vertex_attributes = static_cast<Uint32>(vertex_attributes.size());

    // primitive

    info.primitive_type = static_cast<SDL_GPUPrimitiveType>(m_primitive_topology);

    // rasterizer

    info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    info.rasterizer_state.enable_depth_bias = false;
    info.rasterizer_state.enable_depth_clip = false;

    // multisampling

    info.multisample_state.sample_count = SDL_GPU_SAMPLECOUNT_1; // no multisampling

    // depth stencil

    info.depth_stencil_state.enable_depth_test = false;
    info.depth_stencil_state.enable_depth_write = false;
    info.depth_stencil_state.enable_stencil_test = false;

    // target

    SDL_GPUColorTargetDescription target_description = {};
    // target_description.format = ??;
    target_description.blend_state.src_color_blendfactor = static_cast<SDL_GPUBlendFactor>(m_blend.color_src);
    target_description.blend_state.dst_color_blendfactor = static_cast<SDL_GPUBlendFactor>(m_blend.color_dst);
    target_description.blend_state.color_blend_op = static_cast<SDL_GPUBlendOp>(m_blend.color_op);
    target_description.blend_state.src_alpha_blendfactor = static_cast<SDL_GPUBlendFactor>(m_blend.alpha_src);
    target_description.blend_state.dst_alpha_blendfactor = static_cast<SDL_GPUBlendFactor>(m_blend.alpha_dst);
    target_description.blend_state.alpha_blend_op = static_cast<SDL_GPUBlendOp>(m_blend.alpha_op);
    target_description.blend_state.enable_blend = true;
    target_description.blend_state.enable_color_write_mask = false;

    info.target_info.has_depth_stencil_target = false;
    info.target_info.color_target_descriptions = &target_description;
    info.target_info.num_color_targets = 1;

    // pipeline

    GpuDevice* device = render_manager->device();
    return { *device, SDL_CreateGPUGraphicsPipeline(*device, &info) };
  }

}
