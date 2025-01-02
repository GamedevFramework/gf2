// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/framework/BundleBuilder.h>

#include <gf2/core/AnimationData.h>
#include <gf2/core/AudioSourceData.h>
#include <gf2/core/ConsoleData.h>
#include <gf2/core/FontFace.h>
#include <gf2/core/NinePatchData.h>
#include <gf2/core/RichMapResource.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/TextData.h>

#include <gf2/audio/Music.h>
#include <gf2/audio/Sound.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/RichMap.h>
#include <gf2/framework/SceneSystem.h>

namespace gf {
  BundleBuilder::BundleBuilder(RenderManager* render_manager, FontManager* font_manager, AudioManager* audio_manager)
  : m_render_manager(render_manager)
  , m_font_manager(font_manager)
  , m_audio_manager(audio_manager)
  {
  }

  BundleBuilder::BundleBuilder(SceneSystem* scene_manager)
  : m_render_manager(scene_manager->render_manager())
  , m_font_manager(scene_manager->font_manager())
  , m_audio_manager(scene_manager->audio_manager())
  {
  }

  void BundleBuilder::add_in_bundle(const AnimationResource& resource)
  {
    for (const std::filesystem::path& texture : resource.textures) {
      add_raw_texture(texture);
    }
  }

  void BundleBuilder::add_in_bundle(const AnimationGroupResource& resource)
  {
    for (const std::filesystem::path& texture : resource.textures) {
      add_raw_texture(texture);
    }
  }

  void BundleBuilder::add_in_bundle(const AudioSourceResource& resource)
  {
    switch (resource.data.type) {
      case AudioSourceType::Sound:
        add_raw_sound(resource.filename);
        break;
      case AudioSourceType::Music:
        add_raw_music(resource.filename);
        break;
    }
  }

  void BundleBuilder::add_in_bundle(const ConsoleResource& resource)
  {
    add_raw_console_font(resource.console_font, resource.data);
  }

  void BundleBuilder::add_in_bundle(const NinePatchResource& resource)
  {
    add_raw_texture(resource.texture);
  }

  void BundleBuilder::add_in_bundle(const RichMapResource& resource)
  {
    add_raw_map(resource.filename);
  }

  void BundleBuilder::add_in_bundle(const RichTextResource& resource)
  {
    add_raw_font(resource.default_font);
    add_raw_font(resource.bold_font);
    add_raw_font(resource.italic_font);
    add_raw_font(resource.bold_italic_font);
  }

  void BundleBuilder::add_in_bundle(const SpriteResource& resource)
  {
    add_raw_texture(resource.texture);
  }

  void BundleBuilder::add_in_bundle(const TextResource& resource)
  {
    add_raw_font(resource.font);
  }

  void BundleBuilder::add_raw_texture(const std::filesystem::path& path)
  {
    m_textures.push_back(path);
  }

  void BundleBuilder::add_raw_font(const std::filesystem::path& path)
  {
    m_fonts.push_back(path);
  }

  void BundleBuilder::add_raw_map(const std::filesystem::path& path)
  {
    m_maps.push_back(path);
  }

  void BundleBuilder::add_raw_sound(const std::filesystem::path& path)
  {
    m_sounds.push_back(path);
  }

  void BundleBuilder::add_raw_music(const std::filesystem::path& path)
  {
    m_musics.push_back(path);
  }

  void BundleBuilder::add_raw_console_font(const std::filesystem::path& path, const ConsoleData& data)
  {
    m_console_fonts.push_back({ path, data });
  }


  ResourceBundle BundleBuilder::make_bundle() const
  {
    // capture 'this' by copy in case the bundle builder is a local variable
    ResourceBundle bundle([*this](ResourceBundle* bundle, ResourceManager* resource_manager, ResourceAction action) {
      for (const std::filesystem::path& texture : m_textures) {
        bundle->handle<Texture>(texture, m_render_manager, resource_manager, action);
      }

      for (const std::filesystem::path& font : m_fonts) {
        bundle->handle<FontFace>(font, m_font_manager, resource_manager, action);
      }

      for (const std::filesystem::path& map : m_maps) {
        bundle->handle<RichMap>(map, { m_render_manager, resource_manager }, resource_manager, action);
      }

      for (const std::filesystem::path& sound : m_sounds) {
        bundle->handle<Sound>(sound, m_audio_manager, resource_manager, action);
      }

      for (const std::filesystem::path& music : m_musics) {
        bundle->handle<Music>(music, m_audio_manager, resource_manager, action);
      }

      for (const ConsoleResource& console_resource : m_console_fonts) {
        bundle->handle<ConsoleFont>(console_resource.console_font, { console_resource.data.font_format, console_resource.data.font_size, m_render_manager }, resource_manager, action);
      }

    });

    return bundle;
  }

}
