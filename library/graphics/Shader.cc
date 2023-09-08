// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Shader.h>
// clang-format on

#include <cstdio>

#include <stdexcept>
#include <utility>

#include <fmt/std.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/Renderer.h>

namespace gf {

  namespace {

    std::vector<uint32_t> load_file(const std::filesystem::path& filename)
    {
      std::FILE* file = std::fopen(filename.string().c_str(), "rb");

      if (file == nullptr) {
        Log::error("Unknown shader file: {}.", filename);
        throw std::runtime_error("Unknown shader file: " + filename.string());
      }

      std::fseek(file, 0, SEEK_END);
      auto size = static_cast<std::size_t>(std::ftell(file));
      std::fseek(file, 0, SEEK_SET);

      assert(size % sizeof(uint32_t) == 0);

      std::vector<uint32_t> result(static_cast<std::size_t>(size) / sizeof(uint32_t));

      [[maybe_unused]] const std::size_t size_read = std::fread(result.data(), sizeof(uint32_t), result.size(), file);
      assert(size_read == size);

      std::fclose(file);
      return result;
    }

  }

  Shader::Shader(const std::filesystem::path& filename, ShaderContext context)
  : m_stage(context.stage)
  , m_device(context.renderer->m_device)
  {
    auto code = load_file(filename);
    load_code(code, m_device);
  }

  Shader::Shader(Span<const uint32_t> memory, ShaderContext context)
  : m_stage(context.stage)
  , m_device(context.renderer->m_device)
  {
    load_code(memory, m_device);
  }

  Shader::Shader(Shader&& other) noexcept
  : m_stage(other.m_stage)
  , m_device(std::exchange(other.m_device, VK_NULL_HANDLE))
  , m_shader_module(std::exchange(other.m_shader_module, VK_NULL_HANDLE))
  {
  }

  Shader::~Shader()
  {
    if (m_shader_module != VK_NULL_HANDLE) {
      vkDestroyShaderModule(m_device, m_shader_module, nullptr);
    }
  }

  Shader& Shader::operator=(Shader&& other) noexcept
  {
    std::swap(m_stage, other.m_stage);
    std::swap(m_device, other.m_device);
    std::swap(m_shader_module, other.m_shader_module);
    return *this;
  }

  void Shader::load_code(Span<const uint32_t> code, VkDevice device)
  {
    VkShaderModuleCreateInfo shader_module_create_info = {};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = code.size() * sizeof(uint32_t);
    shader_module_create_info.pCode = code.data();

    if (vkCreateShaderModule(device, &shader_module_create_info, nullptr, &m_shader_module) != VK_SUCCESS) {
      Log::error("Failed to create shader module.");
      throw std::runtime_error("Failed to create shader module.");
    }
  }

}
