// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BUNDLE_BUILDER_H
#define GF_BUNDLE_BUILDER_H

#include <filesystem>
#include <vector>

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ConsoleFontResource.h>
#include <gf2/core/AudioSourceData.h>

#include "FrameworkApi.h"

namespace gf {
  class AudioManager;
  class FontManager;
  class RenderManager;
  class SceneSystem;
  struct AnimationResource;
  struct AnimationGroupResource;
  struct FontResource;
  struct NinePatchResource;
  struct RichTextResource;
  struct ShapeResource;
  struct SpriteResource;
  struct SpriteGroupResource;
  struct TextResource;
  struct TiledMapResource;

  class GF_FRAMEWORK_API BundleBuilder {
  public:
    BundleBuilder(RenderManager* render_manager, FontManager* font_manager, AudioManager* audio_manager);
    BundleBuilder(SceneSystem* scene_manager);

    void add_in_bundle(const AnimationResource& resource);
    void add_in_bundle(const AnimationGroupResource& resource);
    void add_in_bundle(const AudioSourceResource& resource);
    void add_in_bundle(const NinePatchResource& resource);
    void add_in_bundle(const TiledMapResource& resource);
    void add_in_bundle(const RichTextResource& resource);
    void add_in_bundle(const ShapeResource& resource);
    void add_in_bundle(const SpriteResource& resource);
    void add_in_bundle(const SpriteGroupResource& resource);
    void add_in_bundle(const TextResource& resource);

    void add_in_bundle(const MixedConsoleFontResource& resource);
    void add_in_bundle(const ConsoleFontResource& resource);
    void add_in_bundle(const FontResource& resource);

    void add_raw_texture(const std::filesystem::path& path);
    void add_raw_font(const std::filesystem::path& path);
    void add_raw_map(const std::filesystem::path& path);
    void add_raw_sound(const AudioSourceResource& resource);
    void add_raw_music(const AudioSourceResource& resource);

    void add_raw_console_font(const ConsoleFontResource& resource);

    ResourceBundle make_bundle() const;

  private:
    RenderManager* m_render_manager = nullptr;
    FontManager* m_font_manager = nullptr;
    AudioManager* m_audio_manager = nullptr;

    std::vector<std::filesystem::path> m_textures;
    std::vector<std::filesystem::path> m_fonts;
    std::vector<std::filesystem::path> m_maps;
    std::vector<AudioSourceResource> m_sounds;
    std::vector<AudioSourceResource> m_musics;

    std::vector<ConsoleFontResource> m_console_fonts;
  };

}

#endif // GF_BUNDLE_BUILDER_H
