// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Event.h>

namespace gf {

  namespace {

    template<typename T, gf::EventType E>
    constexpr void event_data_check()
    {
      static_assert(Event(0, T{}).type() == E);
      static_assert(Event(0, T{}).is<T>());
      static_assert(std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(Event(0, T{}).from<E>())>>, T>);
    }

    [[maybe_unused]] constexpr void event_check()
    {
      static_assert(Event().type() == gf::EventType::None);
      static_assert(Event(42, std::monostate{}).timestamp() == 42);

      event_data_check<gf::QuitEvent, gf::EventType::Quit>();
      event_data_check<gf::WindowShownEvent, gf::EventType::WindowShown>();
      event_data_check<gf::WindowHiddenEvent, gf::EventType::WindowHidden>();
      event_data_check<gf::WindowExposedEvent, gf::EventType::WindowExposed>();
      event_data_check<gf::WindowMovedEvent, gf::EventType::WindowMoved>();
      event_data_check<gf::WindowResizedEvent, gf::EventType::WindowResized>();
      event_data_check<gf::WindowMinimizedEvent, gf::EventType::WindowMinimized>();
      event_data_check<gf::WindowMaximizedEvent, gf::EventType::WindowMaximized>();
      event_data_check<gf::WindowRestoredEvent, gf::EventType::WindowRestored>();
      event_data_check<gf::MouseFocusGainedEvent, gf::EventType::MouseFocusGained>();
      event_data_check<gf::MouseFocusLostEvent, gf::EventType::MouseFocusLost>();
      event_data_check<gf::KeyboardFocusGainedEvent, gf::EventType::KeyboardFocusGained>();
      event_data_check<gf::KeyboardFocusLostEvent, gf::EventType::KeyboardFocusLost>();
      event_data_check<gf::WindowCloseRequestedEvent, gf::EventType::WindowCloseRequested>();
      event_data_check<gf::KeyPressedEvent, gf::EventType::KeyPressed>();
      event_data_check<gf::KeyRepeatedEvent, gf::EventType::KeyRepeated>();
      event_data_check<gf::KeyReleasedEvent, gf::EventType::KeyReleased>();
      event_data_check<gf::TextEnteredEvent, gf::EventType::TextEntered>();
      event_data_check<gf::MouseMovedEvent, gf::EventType::MouseMoved>();
      event_data_check<gf::MouseButtonPressedEvent, gf::EventType::MouseButtonPressed>();
      event_data_check<gf::MouseButtonReleasedEvent, gf::EventType::MouseButtonReleased>();
      event_data_check<gf::MouseWheelScrolledEvent, gf::EventType::MouseWheelScrolled>();
      event_data_check<gf::GamepadAxisMovedEvent, gf::EventType::GamepadAxisMoved>();
      event_data_check<gf::GamepadButtonPressedEvent, gf::EventType::GamepadButtonPressed>();
      event_data_check<gf::GamepadButtonReleasedEvent, gf::EventType::GamepadButtonReleased>();
      event_data_check<gf::GamepadConnectedEvent, gf::EventType::GamepadConnected>();
      event_data_check<gf::GamepadDisconnectedEvent, gf::EventType::GamepadDisconnected>();
      event_data_check<gf::TouchPressedEvent, gf::EventType::TouchPressed>();
      event_data_check<gf::TouchReleasedEvent, gf::EventType::TouchReleased>();
      event_data_check<gf::TouchMovedEvent, gf::EventType::TouchMoved>();
    }

  }

} // namespace gf
