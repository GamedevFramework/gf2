// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ANIMATION_RUNTIME_H
#define GF_ANIMATION_RUNTIME_H

#include <vector>

#include <gf2/core/AnimationData.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/Time.h>

#include "Texture.h"
#include "Vertex.h"

namespace gf {
  class Buffer;
  class RenderManager;

  namespace details {

    struct AnimationGraphicsFrameRuntime {
      std::size_t texture_index = 0;
      std::size_t offset = 0;
    };

    struct AnimationGraphicsRuntime {
      std::vector<AnimationGraphicsFrameRuntime> frames;
    };

    struct AnimationStateRuntime {
      Flags<AnimationProperties> properties = None;
      std::vector<Time> frames;
    };

    struct AnimationBuilder {
      AnimationGraphicsRuntime append_data(const std::vector<const Texture*>& textures, const AnimationData& data);
      Buffer create_vertices(RenderManager* render_manager);
      Buffer create_indices(RenderManager* render_manager);

      RectF global_bounds = RectF::from_size({ 0.0f, 0.0f });
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
    };

    template<typename Resource>
    std::vector<const Texture*> load_animation_resources(const Resource& resource, ResourceManager* resource_manager)
    {
      std::vector<const Texture*> resources;
      resources.reserve(resource.textures.size());

      for (const auto& texture : resource.textures) {
        resources.push_back(resource_manager->get<Texture>(texture));
      }

      return resources;
    }

    constexpr std::size_t IndicesPerAnimationFrame = 6;
  }
}

#endif // GF_ANIMATION_RUNTIME_H
