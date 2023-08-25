// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Pipeline.h>
// clang-format on

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include <gf2/Log.h>
#include <gf2/Renderer.h>

namespace gf {
  /*
   * Pipeline
   */

  Pipeline::Pipeline(Pipeline&& other) noexcept
  : m_device(std::exchange(other.m_device, VK_NULL_HANDLE))
  , m_pipeline_layout(std::exchange(other.m_pipeline_layout, VK_NULL_HANDLE))
  , m_pipeline(std::exchange(other.m_pipeline, VK_NULL_HANDLE))
  , m_descriptors_layout(std::exchange(other.m_descriptors_layout, VK_NULL_HANDLE))
  {
  }

  Pipeline::~Pipeline()
  {
    if (m_pipeline != VK_NULL_HANDLE) {
      vkDestroyPipeline(m_device, m_pipeline, nullptr);
    }

    if (m_pipeline_layout != VK_NULL_HANDLE) {
      vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
    }

    if (m_descriptors_layout != VK_NULL_HANDLE) {
      vkDestroyDescriptorSetLayout(m_device, m_descriptors_layout, nullptr);
    }
  }

  Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
  {
    std::swap(m_device, other.m_device);
    std::swap(m_pipeline_layout, other.m_pipeline_layout);
    std::swap(m_pipeline, other.m_pipeline);
    std::swap(m_descriptors_layout, other.m_descriptors_layout);
    return *this;
  }

  /*
   * PipelineBuilder
   */

  Pipeline PipelineBuilder::build(Renderer* renderer)
  {
    // shaders

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

    for (auto* shader : m_shaders) {
      VkPipelineShaderStageCreateInfo shader_stage_info = {};
      shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shader_stage_info.stage = static_cast<VkShaderStageFlagBits>(shader->m_stage);
      shader_stage_info.module = shader->m_shader_module;
      shader_stage_info.pName = "main";

      shader_stages.push_back(shader_stage_info);
    }

    // dynamic state

    std::vector<VkDynamicState> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
    dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state_create_info.pDynamicStates = dynamic_states.data();

    // vertex input

    std::vector<VkVertexInputBindingDescription> vertex_bindings;

    std::transform(m_vertex_input.bindings.begin(), m_vertex_input.bindings.end(), std::back_inserter(vertex_bindings), [](auto vertex_binding) {
      VkVertexInputBindingDescription binding_description = {};

      binding_description.binding = vertex_binding.binding;
      binding_description.stride = static_cast<uint32_t>(vertex_binding.stride);
      binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return binding_description;
    });

    std::vector<VkVertexInputAttributeDescription> vertex_attributes;

    std::transform(m_vertex_input.attributes.begin(), m_vertex_input.attributes.end(), std::back_inserter(vertex_attributes), [](auto vertex_attribute) {
      VkVertexInputAttributeDescription attribute_description = {};

      attribute_description.location = vertex_attribute.location;
      attribute_description.binding = vertex_attribute.binding;
      attribute_description.format = static_cast<VkFormat>(vertex_attribute.format);
      attribute_description.offset = static_cast<uint32_t>(vertex_attribute.offset);

      return attribute_description;
    });

    VkPipelineVertexInputStateCreateInfo vertex_input_create_info = {};
    vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_bindings.size());
    vertex_input_create_info.pVertexBindingDescriptions = vertex_bindings.data();
    vertex_input_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attributes.size());
    ;
    vertex_input_create_info.pVertexAttributeDescriptions = vertex_attributes.data();

    // input assembly

    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = {};
    input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology = static_cast<VkPrimitiveTopology>(m_primitive_topology);
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

    // viewport

    VkPipelineViewportStateCreateInfo viewport_state_create_info = {};
    viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.scissorCount = 1;

    // rasterizer

    VkPipelineRasterizationStateCreateInfo rasterizer_state_create_info = {};
    rasterizer_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_state_create_info.depthClampEnable = VK_FALSE;
    rasterizer_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer_state_create_info.lineWidth = 1.0f;
    rasterizer_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_state_create_info.depthBiasEnable = VK_FALSE;

    // multisampling

    VkPipelineMultisampleStateCreateInfo multisampling_state_create_info = {};
    multisampling_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_state_create_info.sampleShadingEnable = VK_FALSE;
    multisampling_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // color blending

    VkPipelineColorBlendAttachmentState color_blend_attachment = {};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_TRUE;
    color_blend_attachment.srcColorBlendFactor = static_cast<VkBlendFactor>(m_blend.color_src);
    color_blend_attachment.dstColorBlendFactor = static_cast<VkBlendFactor>(m_blend.color_dst);
    color_blend_attachment.colorBlendOp = static_cast<VkBlendOp>(m_blend.color_op);
    color_blend_attachment.srcAlphaBlendFactor = static_cast<VkBlendFactor>(m_blend.alpha_src);
    color_blend_attachment.dstAlphaBlendFactor = static_cast<VkBlendFactor>(m_blend.alpha_dst);
    color_blend_attachment.alphaBlendOp = static_cast<VkBlendOp>(m_blend.alpha_op);

    VkPipelineColorBlendStateCreateInfo color_blending_state_create_info = {};
    color_blending_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending_state_create_info.logicOpEnable = VK_FALSE;
    color_blending_state_create_info.logicOp = VK_LOGIC_OP_COPY;
    color_blending_state_create_info.attachmentCount = 1;
    color_blending_state_create_info.pAttachments = &color_blend_attachment;
    color_blending_state_create_info.blendConstants[0] = 0.0f;
    color_blending_state_create_info.blendConstants[1] = 0.0f;
    color_blending_state_create_info.blendConstants[2] = 0.0f;
    color_blending_state_create_info.blendConstants[3] = 0.0f;

    // descriptor set layout

    VkDescriptorSetLayout ds_layout = VK_NULL_HANDLE;
    bool has_ds_layout = false;

    if (!m_descriptor_bindings.empty()) {
      std::vector<VkDescriptorSetLayoutBinding> ds_layout_bindings;

      for (auto descriptor_binding : m_descriptor_bindings) {
        VkDescriptorSetLayoutBinding ds_layout_binding = {};
        ds_layout_binding.binding = descriptor_binding.binding;
        ds_layout_binding.descriptorType = static_cast<VkDescriptorType>(descriptor_binding.type);
        ds_layout_binding.descriptorCount = 1;
        ds_layout_binding.stageFlags = static_cast<VkShaderStageFlags>(descriptor_binding.stage);

        ds_layout_bindings.push_back(ds_layout_binding);
      }

      VkDescriptorSetLayoutCreateInfo ds_layout_info = {};
      ds_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      ds_layout_info.bindingCount = static_cast<uint32_t>(ds_layout_bindings.size());
      ds_layout_info.pBindings = ds_layout_bindings.data();

      if (vkCreateDescriptorSetLayout(renderer->m_device, &ds_layout_info, nullptr, &ds_layout) != VK_SUCCESS) {
        Log::error("Failed to create descriptor set layout.");
        throw std::runtime_error("Failed to create descriptor set layout.");
      }

      has_ds_layout = true;
    }

    // pipeline layout

    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount = has_ds_layout ? 1 : 0;
    pipeline_layout_create_info.pSetLayouts = has_ds_layout ? &ds_layout : nullptr;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges = nullptr;

    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;

    if (vkCreatePipelineLayout(renderer->m_device, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
      Log::error("Failed to create pipeline layout.");
      throw std::runtime_error("Failed to create pipeline layout.");
    }

    // pipeline

    VkGraphicsPipelineCreateInfo pipeline_create_info = {};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.stageCount = static_cast<uint32_t>(shader_stages.size());
    pipeline_create_info.pStages = shader_stages.data();

    pipeline_create_info.pVertexInputState = &vertex_input_create_info;
    pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
    pipeline_create_info.pViewportState = &viewport_state_create_info;
    pipeline_create_info.pRasterizationState = &rasterizer_state_create_info;
    pipeline_create_info.pMultisampleState = &multisampling_state_create_info;
    pipeline_create_info.pDepthStencilState = nullptr;
    pipeline_create_info.pColorBlendState = &color_blending_state_create_info;
    pipeline_create_info.pDynamicState = &dynamic_state_create_info;

    pipeline_create_info.layout = pipeline_layout;

    VkPipeline pipeline = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(renderer->m_device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline) != VK_SUCCESS) {
      throw std::runtime_error("failed to create graphics pipeline!");
    }

    return { renderer->m_device, pipeline_layout, pipeline, ds_layout };
  }

}
