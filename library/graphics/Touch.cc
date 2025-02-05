// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Touch.h>

#include <memory>
#include <type_traits>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>
#include <gf2/core/TouchTypes.h>

namespace gf {

  namespace {
    static_assert(std::is_same_v<std::underlying_type_t<TouchId>, SDL_TouchID>);
    static_assert(MouseTouchId == static_cast<TouchId>(SDL_MOUSE_TOUCHID));
    static_assert(std::is_same_v<std::underlying_type_t<TouchDeviceType>, std::underlying_type_t<SDL_TouchDeviceType>>);

    static_assert(std::is_same_v<std::underlying_type_t<FingerId>, SDL_FingerID>);

    template<TouchDeviceType Type, SDL_TouchDeviceType Value>
    constexpr void touch_device_type_check()
    {
      static_assert(static_cast<SDL_TouchDeviceType>(Type) == Value);
    }

    [[maybe_unused]] constexpr void touch_check()
    {
      touch_device_type_check<TouchDeviceType::Invalid, SDL_TOUCH_DEVICE_INVALID>();
      touch_device_type_check<TouchDeviceType::Direct, SDL_TOUCH_DEVICE_DIRECT>();
      touch_device_type_check<TouchDeviceType::IndirectAbsolute, SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE>();
      touch_device_type_check<TouchDeviceType::IndirectRelative, SDL_TOUCH_DEVICE_INDIRECT_RELATIVE>();
    }

    struct TouchIdDeleter {
      void operator()(SDL_TouchID* ids)
      {
        SDL_free(ids);
      }
    };

  } // namespace

  std::vector<TouchId> Touch::devices()
  {
    int count = 0;
    std::unique_ptr<SDL_TouchID[], TouchIdDeleter> raw_devices(SDL_GetTouchDevices(&count));

    std::vector<TouchId> devices;
    devices.reserve(count);

    for (int i = 0; i < count; ++i) {
      devices.push_back(TouchId{ raw_devices[i] });
    }

    return devices;
  }

  ZString Touch::name(TouchId id)
  {
    if (const char* name = SDL_GetTouchDeviceName(static_cast<SDL_TouchID>(id)); name != nullptr) {
      return name;
    }

    Log::error("Failed to get touch name: {}", SDL_GetError());
    return "";
  }

  TouchDeviceType Touch::type(TouchId id)
  {
    return TouchDeviceType{ SDL_GetTouchDeviceType(static_cast<SDL_TouchID>(id)) };
  }

} // namespace gf
