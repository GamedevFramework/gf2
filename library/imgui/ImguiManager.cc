// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/imgui/ImguiManager.h>

#include <imgui.h>

#include <gf2/graphics/Clipboard.h>

namespace gf {

  namespace {

    const char* get_clipboard_text([[maybe_unused]] void* user_data)
    {
      static std::string g_clipboard;
      g_clipboard = gf::Clipboard::text();
      return g_clipboard.c_str();
    }

    void set_clipboard_text([[maybe_unused]] void* user_data, const char* text)
    {
      gf::Clipboard::set_text(text);
    }

    CursorType to_cursor_type(ImGuiMouseCursor cursor)
    {
      switch (cursor) {
        case ImGuiMouseCursor_Arrow:
          return CursorType::Arrow;
        case ImGuiMouseCursor_TextInput:
          return CursorType::Text;
        case ImGuiMouseCursor_ResizeAll:
          return CursorType::SizeAll;
        case ImGuiMouseCursor_ResizeNS:
          return CursorType::SizeVertical;
        case ImGuiMouseCursor_ResizeEW:
          return CursorType::SizeHorizontal;
        case ImGuiMouseCursor_ResizeNESW:
          return CursorType::SizeBottomLeftTopRight;
        case ImGuiMouseCursor_ResizeNWSE:
          return CursorType::SizeTopLeftBottomRight;
        case ImGuiMouseCursor_Hand:
          return CursorType::Hand;
        case ImGuiMouseCursor_NotAllowed:
          return CursorType::NotAllowed;
        default:
          break;
      }

      return CursorType::Arrow;
    }

  }

  ImguiManager::ImguiManager(Window* window, RenderManager* render_manager)
  {
    ImGuiIO& io = ImGui::GetIO();

    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    io.BackendPlatformName = "imgui_impl_gf2";

    io.SetClipboardTextFn = set_clipboard_text;
    io.GetClipboardTextFn = get_clipboard_text;
    io.ClipboardUserData = nullptr;

    io.BackendPlatformUserData = static_cast<void*>(window);

    io.BackendRendererName = "imgui_impl_gf2";

    Vec2I size = {};
    unsigned char* pixels = nullptr;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &size.w, &size.h);
    const Image font_image(size, pixels, PixelFormat::Rgba32);
    m_font_texture = Texture(font_image, render_manager);
    m_font_texture.set_debug_name("[gf2] Imgui Font Texture");
    io.Fonts->TexID = reinterpret_cast<uintptr_t>(&m_font_texture); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)

    io.BackendRendererUserData = static_cast<void*>(render_manager);
  }

  ImguiManager::~ImguiManager()
  {
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererUserData = nullptr;
    io.BackendPlatformUserData = nullptr;
  }

  bool ImguiManager::process_event(const Event& event)
  {
    ImGuiIO& io = ImGui::GetIO();

    switch (event.type) {
      case EventType::KeyPressed:
        update_keyboard_key(event.key.keycode, event.key.modifiers, true);
        return io.WantCaptureKeyboard;

      case EventType::KeyReleased:
        update_keyboard_key(event.key.keycode, event.key.modifiers, false);
        return io.WantCaptureKeyboard;

      case gf::EventType::MouseButtonPressed:
        io.AddMouseSourceEvent(event.mouse_button.mouse_id == TouchMouseId ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
        update_mouse_button(event.mouse_button.button, true);
        return io.WantCaptureMouse;

      case gf::EventType::MouseButtonReleased:
        io.AddMouseSourceEvent(event.mouse_button.mouse_id == TouchMouseId ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
        update_mouse_button(event.mouse_button.button, false);
        return io.WantCaptureMouse;

      case EventType::MouseMoved:
        {
          const Vec2F coordinates = event.mouse_motion.position;
          io.AddMouseSourceEvent(event.mouse_motion.mouse_id == TouchMouseId ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
          io.AddMousePosEvent(coordinates.x, coordinates.y);
          return io.WantCaptureMouse;
        }

      case EventType::MouseWheelScrolled:
        {
          const Vec2F offset = event.mouse_wheel.offset;
          io.AddMouseSourceEvent(event.mouse_wheel.mouse_id == TouchMouseId ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
          io.AddMouseWheelEvent(-offset.x, offset.y);
          return io.WantCaptureMouse;
        }

      case EventType::GamepadButtonPressed:
        udpate_gamepad_button(event.gamepad_button.button, true, 1.0f);
        return true;

      case EventType::GamepadButtonReleased:
        udpate_gamepad_button(event.gamepad_button.button, false, 1.0f);
        return true;

      case EventType::GamepadAxisMoved:
        update_gamepad_axis(event.gamepad_axis.axis, event.gamepad_axis.value);
        return true;

      case EventType::TextEntered:
        io.AddInputCharactersUTF8(event.text.text.data());
        return io.WantCaptureKeyboard;

      default:
        // nothing to do
        break;
    }

    return false;
  }

  void ImguiManager::update(Time time)
  {
    ImGuiIO& io = ImGui::GetIO();
    auto* window = static_cast<Window*>(io.BackendPlatformUserData);

    const Vec2F size = window->size();
    io.DisplaySize = ImVec2(size.x, size.y);

    if (size.x > 0 && size.y > 0) {
      const Vec2F scale = window->surface_size() / size;
      io.DisplayFramebufferScale = ImVec2(scale.x, scale.y);
    }

    io.DeltaTime = time.as_seconds();

    update_mouse_cursor();
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void ImguiManager::update_keyboard_key(Keycode keycode, Flags<Modifier> modifiers, bool pressed)
  {
    ImGuiIO& io = ImGui::GetIO();

    switch (keycode) {
      case Keycode::Tab:
        io.AddKeyEvent(ImGuiKey_Tab, pressed);
        break;
      case Keycode::Left:
        io.AddKeyEvent(ImGuiKey_LeftArrow, pressed);
        break;
      case Keycode::Right:
        io.AddKeyEvent(ImGuiKey_RightArrow, pressed);
        break;
      case Keycode::Up:
        io.AddKeyEvent(ImGuiKey_UpArrow, pressed);
        break;
      case Keycode::Down:
        io.AddKeyEvent(ImGuiKey_DownArrow, pressed);
        break;
      case Keycode::PageUp:
        io.AddKeyEvent(ImGuiKey_PageUp, pressed);
        break;
      case Keycode::PageDown:
        io.AddKeyEvent(ImGuiKey_PageDown, pressed);
        break;
      case Keycode::Home:
        io.AddKeyEvent(ImGuiKey_Home, pressed);
        break;
      case Keycode::End:
        io.AddKeyEvent(ImGuiKey_End, pressed);
        break;
      case Keycode::Insert:
        io.AddKeyEvent(ImGuiKey_Insert, pressed);
        break;
      case Keycode::Delete:
        io.AddKeyEvent(ImGuiKey_Delete, pressed);
        break;
      case Keycode::Backspace:
        io.AddKeyEvent(ImGuiKey_Backspace, pressed);
        break;
      case Keycode::Space:
        io.AddKeyEvent(ImGuiKey_Space, pressed);
        break;
      case Keycode::Return:
        io.AddKeyEvent(ImGuiKey_Enter, pressed);
        break;
      case Keycode::Escape:
        io.AddKeyEvent(ImGuiKey_Escape, pressed);
        break;
      case Keycode::NumpadEnter:
        io.AddKeyEvent(ImGuiKey_KeypadEnter, pressed);
        break;
      case Keycode::A:
        io.AddKeyEvent(ImGuiKey_A, pressed);
        break;
      case Keycode::C:
        io.AddKeyEvent(ImGuiKey_C, pressed);
        break;
      case Keycode::V:
        io.AddKeyEvent(ImGuiKey_V, pressed);
        break;
      case Keycode::X:
        io.AddKeyEvent(ImGuiKey_X, pressed);
        break;
      case Keycode::Y:
        io.AddKeyEvent(ImGuiKey_Y, pressed);
        break;
      case Keycode::Z:
        io.AddKeyEvent(ImGuiKey_Z, pressed);
        break;
      default:
        // nothing to do
        break;
    }

    io.KeyCtrl = modifiers.test(Modifier::Ctrl);
    io.KeyShift = modifiers.test(Modifier::Shift);
    io.KeyAlt = modifiers.test(Modifier::Alt);
    io.KeySuper = modifiers.test(Modifier::Gui);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void ImguiManager::update_mouse_button(MouseButton button, bool pressed)
  {
    ImGuiIO& io = ImGui::GetIO();

    switch (button) {
      case MouseButton::Left:
        io.AddMouseButtonEvent(ImGuiMouseButton_Left, pressed);
        break;
      case MouseButton::Middle:
        io.AddMouseButtonEvent(ImGuiMouseButton_Middle, pressed);
        break;
      case MouseButton::Right:
        io.AddMouseButtonEvent(ImGuiMouseButton_Right, pressed);
        break;
      default:
        // nothing to do
        break;
    }
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void ImguiManager::udpate_gamepad_button(GamepadButton button, bool pressed, float value)
  {
    ImGuiIO& io = ImGui::GetIO();

    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) {
      return;
    }

    switch (button) {
      case GamepadButton::A:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceDown, pressed, value);
        break;
      case GamepadButton::B:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceRight, pressed, value);
        break;
      case GamepadButton::X:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceLeft, pressed, value);
        break;
      case GamepadButton::Y:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceUp, pressed, value);
        break;
      case GamepadButton::DPadLeft:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadLeft, pressed, value);
        break;
      case GamepadButton::DPadRight:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadRight, pressed, value);
        break;
      case GamepadButton::DPadUp:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadUp, pressed, value);
        break;
      case GamepadButton::DPadDown:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadDown, pressed, value);
        break;
      case GamepadButton::LeftBumper:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadL1, pressed, value);
        break;
      case GamepadButton::RightBumper:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadR1, pressed, value);
        break;
      default:
        // nothing to do
        break;
    }
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void ImguiManager::update_gamepad_axis(GamepadAxis axis, int16_t raw)
  {
    ImGuiIO& io = ImGui::GetIO();

    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) {
      return;
    }

    static constexpr int16_t Threshold = 8000;

    switch (axis) {
      case GamepadAxis::LeftX:
        if (raw > Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, true, 1.0f);
        } else if (raw < -Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, true, 1.0f);
        } else {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, false, 0.0f);
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, false, 0.0f);
        }
        break;
      case GamepadAxis::LeftY:
        if (raw > Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, true, 1.0f);
        } else if (raw < -Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, true, 1.0f);
        } else {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, false, 0.0f);
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, false, 0.0f);
        }
        break;
      default:
        // nothing to do
        break;
    }
  }

  void ImguiManager::update_mouse_cursor()
  {
    const ImGuiIO& io = ImGui::GetIO();

    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) != 0) {
      return;
    }

    const ImGuiMouseCursor cursor = ImGui::GetMouseCursor();

    if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
      Cursor::set_mouse_cursor_visible(false);
    } else {
      const CursorType cursor_type = to_cursor_type(cursor);

      if (cursor_type == m_cursor_type) {
        return;
      }

      m_cursor = Cursor(cursor_type);
      Cursor::set_mouse_cursor_visible(true);
      Cursor::set_mouse_cursor(&m_cursor);
      m_cursor_type = cursor_type;
    }
  }

}
