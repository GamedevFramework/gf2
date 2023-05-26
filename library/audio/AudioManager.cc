// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/AudioManager.h>
// clang-format on

#include <cassert>
#include <cstring>

#include <array>

#include <SDL2/SDL_audio.h>
#include <gf2/Log.h>

// clang-format: off
#define GF_AUDIO_BACKEND_IMPLEMENTATION
#include "bits/AudioBackend.h"
// clang-format on

namespace gf {

  struct sdl_device : ma_device {
    int device_id_playback;
    int device_id_capture;
  };

  namespace {

    ma_format to_miniaudio(SDL_AudioFormat format)
    {
      switch (format) {
        case AUDIO_U8:
          return ma_format_u8;
        case AUDIO_S16:
          return ma_format_s16;
        case AUDIO_S32:
          return ma_format_s32;
        case AUDIO_F32:
          return ma_format_f32;
        default:
          return ma_format_unknown;
      }
    }

    SDL_AudioFormat to_sdl(ma_format format)
    {
      switch (format) {
        case ma_format_unknown:
          return 0;
        case ma_format_u8:
          return AUDIO_U8;
        case ma_format_s16:
          return AUDIO_S16;
        case ma_format_s24:
          return AUDIO_S32; // Closest match
        case ma_format_s32:
          return AUDIO_S32;
        case ma_format_f32:
          return AUDIO_F32;
        default:
          return 0;
      }
    }

    ma_result sdl_context_enumerate_devices(ma_context* context, ma_enum_devices_callback_proc callback, void* user_data)
    {
      assert(context != nullptr);
      assert(callback != nullptr);

      bool finished = false;

      /* Playback */
      if (!finished) {
        int device_count = SDL_GetNumAudioDevices(0);

        for (int i = 0; i < device_count; ++i) {
          ma_device_info device_info = {};
          MA_ZERO_OBJECT(&device_info);

          device_info.id.custom.i = i;
          std::strncpy(device_info.name, SDL_GetAudioDeviceName(i, 0), sizeof(device_info.name));
          device_info.name[sizeof(device_info.name) - 1] = '\0';

          if (i == 0) {
            device_info.isDefault = MA_TRUE;
          }

          auto result = callback(context, ma_device_type_playback, &device_info, user_data);

          if (result == MA_FALSE) {
            finished = true;
            break;
          }
        }
      }

      /* Capture */
      if (!finished) {
        int device_count = SDL_GetNumAudioDevices(1);

        for (int i = 0; i < device_count; ++i) {
          ma_device_info device_info = {};
          MA_ZERO_OBJECT(&device_info);

          device_info.id.custom.i = i;
          std::strncpy(device_info.name, SDL_GetAudioDeviceName(i, 0), sizeof(device_info.name));
          device_info.name[sizeof(device_info.name) - 1] = '\0';

          if (i == 0) {
            device_info.isDefault = MA_TRUE;
          }

          auto result = callback(context, ma_device_type_capture, &device_info, user_data);

          if (result == MA_FALSE) {
            finished = true;
            break;
          }
        }
      }

      return MA_SUCCESS;
    }

    ma_result sdl_context_get_device_info([[maybe_unused]] ma_context* context, ma_device_type device, const ma_device_id* device_id, ma_device_info* device_info)
    {
      assert(context != nullptr);
      const int is_capture = (device == ma_device_type_playback) ? 0 : 1;

      if (device_id == nullptr) {
        if (device == ma_device_type_playback) {
          device_info->id.custom.i = 0;
          std::strncpy(device_info->name, MA_DEFAULT_PLAYBACK_DEVICE_NAME, sizeof(device_info->name));
        } else {
          device_info->id.custom.i = 0;
          std::strncpy(device_info->name, MA_DEFAULT_CAPTURE_DEVICE_NAME, sizeof(device_info->name));
        }
      } else {
        device_info->id.custom.i = device_id->custom.i;
        std::strncpy(device_info->name, SDL_GetAudioDeviceName(device_id->custom.i, is_capture), sizeof(device_info->name));
        device_info->name[sizeof(device_info->name) - 1] = '\0';
      }

      if (device_info->id.custom.i == 0) {
        device_info->isDefault = MA_TRUE;
      }

      const char* device_name = nullptr;

      if (device_id != nullptr) {
        device_name = SDL_GetAudioDeviceName(device_id->custom.i, is_capture);
      }

      SDL_AudioSpec desired;
      MA_ZERO_MEMORY(&desired, sizeof(desired));
      SDL_AudioSpec obtained;

      auto id = SDL_OpenAudioDevice(device_name, is_capture, &desired, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);

      if (id == 0) {
        Log::error("Failed to open SDL audio device.");
        return MA_FAILED_TO_OPEN_BACKEND_DEVICE;
      }

      SDL_CloseAudioDevice(id);

      device_info->nativeDataFormatCount = 1;
      device_info->nativeDataFormats[0].format = to_miniaudio(obtained.format);
      device_info->nativeDataFormats[0].channels = obtained.channels;
      device_info->nativeDataFormats[0].sampleRate = obtained.freq;
      device_info->nativeDataFormats[0].flags = 0;

      if (device_info->nativeDataFormats[0].format == ma_format_unknown) {
        device_info->nativeDataFormats[0].format = ma_format_f32;
      }

      return MA_SUCCESS;
    }

    void sdl_audio_callback_capture(void* user_data, ma_uint8* buffer, int buffer_size)
    {
      auto device = static_cast<ma_device*>(user_data);
      assert(device != NULL);
      const ma_uint32 bytes_per_frame = ma_get_bytes_per_frame(device->capture.internalFormat, device->capture.internalChannels);
      const ma_uint32 frame_count = static_cast<ma_uint32>(buffer_size / bytes_per_frame);
      ma_device_handle_backend_data_callback(device, nullptr, buffer, frame_count);
    }

    void sdl_audio_callback_playback(void* user_data, ma_uint8* buffer, int buffer_size)
    {
      auto device = static_cast<ma_device*>(user_data);
      assert(device != NULL);
      const ma_uint32 bytes_per_frame = ma_get_bytes_per_frame(device->playback.internalFormat, device->playback.internalChannels);
      const ma_uint32 frame_count = static_cast<ma_uint32>(buffer_size / bytes_per_frame);
      ma_device_handle_backend_data_callback(device, buffer, nullptr, frame_count);
    }

    ma_result sdl_device_init_ex(ma_device* raw_device, const ma_device_config* config, ma_device_descriptor* descriptor)
    {
      auto device = static_cast<sdl_device*>(raw_device);
      assert(device != nullptr);
      assert(descriptor != nullptr);

      if (descriptor->sampleRate == 0) {
        descriptor->sampleRate = MA_DEFAULT_SAMPLE_RATE;
      }

      descriptor->periodSizeInFrames = ma_calculate_buffer_size_in_frames_from_descriptor(descriptor, descriptor->sampleRate, config->performanceProfile);

      if (descriptor->periodSizeInFrames > 32768) {
        descriptor->periodSizeInFrames = 32768;
      } else {
        descriptor->periodSizeInFrames = ma_next_power_of_2(descriptor->periodSizeInFrames);
      }

      SDL_AudioSpec desired;

      MA_ZERO_OBJECT(&desired);
      desired.freq = static_cast<int>(descriptor->sampleRate);
      desired.format = to_sdl(descriptor->format);
      desired.channels = static_cast<ma_uint8>(descriptor->channels);
      desired.samples = static_cast<ma_uint16>(descriptor->periodSizeInFrames);
      desired.callback = (config->deviceType == ma_device_type_capture) ? sdl_audio_callback_capture : sdl_audio_callback_playback;
      desired.userdata = device;

      if (desired.format == 0) {
        desired.format = AUDIO_F32;
      }

      const char* device_name = nullptr;

      if (descriptor->pDeviceID != NULL) {
        device_name = SDL_GetAudioDeviceName(descriptor->pDeviceID->custom.i, (config->deviceType == ma_device_type_playback) ? 0 : 1);
      }

      SDL_AudioSpec obtained;

      auto id = SDL_OpenAudioDevice(device_name, (config->deviceType == ma_device_type_playback) ? 0 : 1, &desired, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);

      if (id == 0) {
        Log::error("Failed to open SDL audio device.");
        return MA_FAILED_TO_OPEN_BACKEND_DEVICE;
      }

      if (config->deviceType == ma_device_type_playback) {
        device->device_id_playback = id;
      } else {
        device->device_id_capture = id;
      }

      descriptor->format = to_miniaudio(obtained.format);
      descriptor->channels = obtained.channels;
      descriptor->sampleRate = static_cast<ma_uint32>(obtained.freq);
      ma_channel_map_init_standard(ma_standard_channel_map_default, descriptor->channelMap, ma_countof(descriptor->channelMap), descriptor->channels);
      descriptor->periodSizeInFrames = obtained.samples;
      descriptor->periodCount = 1;

      return MA_SUCCESS;
    }

    ma_result sdl_device_init(ma_device* raw_device, const ma_device_config* config, ma_device_descriptor* descriptor_playback, ma_device_descriptor* descriptor_capture)
    {
      auto device = static_cast<sdl_device*>(raw_device);
      assert(device != nullptr);

      if (config->deviceType == ma_device_type_loopback) {
        return MA_DEVICE_TYPE_NOT_SUPPORTED;
      }

      if (config->deviceType == ma_device_type_capture || config->deviceType == ma_device_type_duplex) {
        auto result = sdl_device_init_ex(device, config, descriptor_capture);

        if (result != MA_SUCCESS) {
          return result;
        }
      }

      if (config->deviceType == ma_device_type_playback || config->deviceType == ma_device_type_duplex) {
        auto result = sdl_device_init_ex(device, config, descriptor_playback);

        if (result != MA_SUCCESS) {
          if (config->deviceType == ma_device_type_duplex) {
            SDL_CloseAudioDevice(device->device_id_capture);
          }

          return result;
        }
      }

      return MA_SUCCESS;
    }

    static ma_result sdl_device_uninit(ma_device* raw_device)
    {
      auto device = static_cast<sdl_device*>(raw_device);
      assert(device != NULL);

      if (device->type == ma_device_type_capture || device->type == ma_device_type_duplex) {
        SDL_CloseAudioDevice(device->device_id_capture);
      }

      if (device->type == ma_device_type_playback || device->type == ma_device_type_duplex) {
        SDL_CloseAudioDevice(device->device_id_playback);
      }

      return MA_SUCCESS;
    }

    ma_result sdl_device_start(ma_device* raw_device)
    {
      auto device = static_cast<sdl_device*>(raw_device);
      assert(device != NULL);

      if (device->type == ma_device_type_capture || device->type == ma_device_type_duplex) {
        SDL_PauseAudioDevice(device->device_id_capture, 0);
      }

      if (device->type == ma_device_type_playback || device->type == ma_device_type_duplex) {
        SDL_PauseAudioDevice(device->device_id_playback, 0);
      }

      return MA_SUCCESS;
    }

    ma_result sdl_device_stop(ma_device* raw_device)
    {
      auto device = static_cast<sdl_device*>(raw_device);
      assert(device != NULL);

      if (device->type == ma_device_type_capture || device->type == ma_device_type_duplex) {
        SDL_PauseAudioDevice(device->device_id_capture, 1);
      }

      if (device->type == ma_device_type_playback || device->type == ma_device_type_duplex) {
        SDL_PauseAudioDevice(device->device_id_playback, 1);
      }

      return MA_SUCCESS;
    }

    ma_result sdl_context_uninit([[maybe_unused]] ma_context* context)
    {
      return MA_SUCCESS;
    }

    static ma_result sdl_context_init([[maybe_unused]] ma_context* context, [[maybe_unused]] const ma_context_config* config, ma_backend_callbacks* callbacks)
    {
      assert(context != NULL);

      callbacks->onContextInit = sdl_context_init;
      callbacks->onContextUninit = sdl_context_uninit;
      callbacks->onContextEnumerateDevices = sdl_context_enumerate_devices;
      callbacks->onContextGetDeviceInfo = sdl_context_get_device_info;
      callbacks->onDeviceInit = sdl_device_init;
      callbacks->onDeviceUninit = sdl_device_uninit;
      callbacks->onDeviceStart = sdl_device_start;
      callbacks->onDeviceStop = sdl_device_stop;

      return MA_SUCCESS;
    }

  }

  struct AudioManager::AudioSystem {
    ma_context context;
    sdl_device device;
    ma_engine engine;
  };

  AudioManager::AudioManager()
  : m_system(std::make_unique<AudioSystem>())
  {
    using namespace std::literals;
    assert(ma_is_backend_enabled(ma_backend_custom) == MA_TRUE);

    ma_backend backends[] = {
      ma_backend_custom
    };

    ma_context_config context_config = ma_context_config_init();
    context_config.custom.onContextInit = sdl_context_init;

    if (auto result = ma_context_init(std::begin(backends), std::size(backends), &context_config, &m_system->context); result != MA_SUCCESS) {
      Log::error("Unable to initialize audio context: {}", ma_result_description(result));
      throw std::runtime_error("Unable to initialize audio context: "s + ma_result_description(result));
    }

    ma_device_config device_config = ma_device_config_init(ma_device_type_playback);

    if (auto result = ma_device_init(&m_system->context, &device_config, &m_system->device); result != MA_SUCCESS) {
      Log::error("Unable to initialize audio device: {}", ma_result_description(result));
      throw std::runtime_error("Unable to initialize audio device: "s + ma_result_description(result));
    }

    ma_engine_config engine_config = ma_engine_config_init();
    engine_config.pDevice = &m_system->device;

    if (auto result = ma_engine_init(&engine_config, &m_system->engine); result != MA_SUCCESS) {
      Log::error("Unable to initialize audio engine: {}", ma_result_description(result));
      throw std::runtime_error("Unable to initialize audio engine: "s + ma_result_description(result));
    }
  }

  AudioManager::AudioManager(AudioManager&& other) noexcept = default;

  AudioManager::~AudioManager()
  {
    ma_engine_uninit(&m_system->engine);
    ma_device_uninit(&m_system->device);
    ma_context_uninit(&m_system->context);
  }

  AudioManager& AudioManager::operator=(AudioManager&& other) noexcept = default;

  std::string AudioManager::device_name() const
  {
    std::array<char, MA_MAX_DEVICE_NAME_LENGTH + 1> buffer = {};
    ma_device_get_name(&m_system->device, ma_device_type_playback, buffer.data(), buffer.size(), nullptr);
    return buffer.data();
  }

  ma_engine* AudioManager::engine()
  {
    return &m_system->engine;
  }

}
