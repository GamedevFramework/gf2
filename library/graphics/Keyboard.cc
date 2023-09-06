// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Keyboard.h>
// clang-format on

#include <SDL2/SDL.h>

namespace gf {

  namespace {

    template<Modifier Mod, SDL_Keymod Value>
    constexpr void modifier_check()
    {
      static_assert(static_cast<SDL_Keymod>(Mod) == Value, "Check gf::Modifier");
    }

    template<Scancode Code, SDL_Scancode Value>
    constexpr void scancode_check()
    {
      static_assert(static_cast<SDL_Scancode>(Code) == Value, "Check gf::Scancode");
    }

    template<Keycode Code, SDL_Keycode Value>
    constexpr void keycode_check()
    {
      static_assert(static_cast<SDL_Keycode>(Code) == Value, "Check gf::Keycode");
    }

    [[maybe_unused]] constexpr void keyboard_check()
    {
      modifier_check<Modifier::None, KMOD_NONE>();
      modifier_check<Modifier::LeftShift, KMOD_LSHIFT>();
      modifier_check<Modifier::RightShift, KMOD_RSHIFT>();
      modifier_check<Modifier::LeftCtrl, KMOD_LCTRL>();
      modifier_check<Modifier::RightCtrl, KMOD_RCTRL>();
      modifier_check<Modifier::LeftAlt, KMOD_LALT>();
      modifier_check<Modifier::RightAlt, KMOD_RALT>();
      modifier_check<Modifier::LeftGui, KMOD_LGUI>();
      modifier_check<Modifier::RightGui, KMOD_RGUI>();
      modifier_check<Modifier::Num, KMOD_NUM>();
      modifier_check<Modifier::Caps, KMOD_CAPS>();
      modifier_check<Modifier::Mode, KMOD_MODE>();
      modifier_check<Modifier::Shift, KMOD_SHIFT>();
      modifier_check<Modifier::Ctrl, KMOD_CTRL>();
      modifier_check<Modifier::Alt, KMOD_ALT>();
      modifier_check<Modifier::Gui, KMOD_GUI>();

      scancode_check<Scancode::Unknown, SDL_SCANCODE_UNKNOWN>();
      scancode_check<Scancode::A, SDL_SCANCODE_A>();
      scancode_check<Scancode::B, SDL_SCANCODE_B>();
      scancode_check<Scancode::C, SDL_SCANCODE_C>();
      scancode_check<Scancode::D, SDL_SCANCODE_D>();
      scancode_check<Scancode::E, SDL_SCANCODE_E>();
      scancode_check<Scancode::F, SDL_SCANCODE_F>();
      scancode_check<Scancode::G, SDL_SCANCODE_G>();
      scancode_check<Scancode::H, SDL_SCANCODE_H>();
      scancode_check<Scancode::I, SDL_SCANCODE_I>();
      scancode_check<Scancode::J, SDL_SCANCODE_J>();
      scancode_check<Scancode::K, SDL_SCANCODE_K>();
      scancode_check<Scancode::L, SDL_SCANCODE_L>();
      scancode_check<Scancode::M, SDL_SCANCODE_M>();
      scancode_check<Scancode::N, SDL_SCANCODE_N>();
      scancode_check<Scancode::O, SDL_SCANCODE_O>();
      scancode_check<Scancode::P, SDL_SCANCODE_P>();
      scancode_check<Scancode::Q, SDL_SCANCODE_Q>();
      scancode_check<Scancode::R, SDL_SCANCODE_R>();
      scancode_check<Scancode::S, SDL_SCANCODE_S>();
      scancode_check<Scancode::T, SDL_SCANCODE_T>();
      scancode_check<Scancode::U, SDL_SCANCODE_U>();
      scancode_check<Scancode::V, SDL_SCANCODE_V>();
      scancode_check<Scancode::W, SDL_SCANCODE_W>();
      scancode_check<Scancode::X, SDL_SCANCODE_X>();
      scancode_check<Scancode::Y, SDL_SCANCODE_Y>();
      scancode_check<Scancode::Z, SDL_SCANCODE_Z>();
      scancode_check<Scancode::Num1, SDL_SCANCODE_1>();
      scancode_check<Scancode::Num2, SDL_SCANCODE_2>();
      scancode_check<Scancode::Num3, SDL_SCANCODE_3>();
      scancode_check<Scancode::Num4, SDL_SCANCODE_4>();
      scancode_check<Scancode::Num5, SDL_SCANCODE_5>();
      scancode_check<Scancode::Num6, SDL_SCANCODE_6>();
      scancode_check<Scancode::Num7, SDL_SCANCODE_7>();
      scancode_check<Scancode::Num8, SDL_SCANCODE_8>();
      scancode_check<Scancode::Num9, SDL_SCANCODE_9>();
      scancode_check<Scancode::Num0, SDL_SCANCODE_0>();
      scancode_check<Scancode::Return, SDL_SCANCODE_RETURN>();
      scancode_check<Scancode::Escape, SDL_SCANCODE_ESCAPE>();
      scancode_check<Scancode::Backspace, SDL_SCANCODE_BACKSPACE>();
      scancode_check<Scancode::Tab, SDL_SCANCODE_TAB>();
      scancode_check<Scancode::Space, SDL_SCANCODE_SPACE>();
      scancode_check<Scancode::Minus, SDL_SCANCODE_MINUS>();
      scancode_check<Scancode::Equals, SDL_SCANCODE_EQUALS>();
      scancode_check<Scancode::LeftBracket, SDL_SCANCODE_LEFTBRACKET>();
      scancode_check<Scancode::RightBracket, SDL_SCANCODE_RIGHTBRACKET>();
      scancode_check<Scancode::NonUsHash, SDL_SCANCODE_NONUSHASH>();
      scancode_check<Scancode::Semicolon, SDL_SCANCODE_SEMICOLON>();
      scancode_check<Scancode::Apostrophe, SDL_SCANCODE_APOSTROPHE>();
      scancode_check<Scancode::Grave, SDL_SCANCODE_GRAVE>();
      scancode_check<Scancode::Comma, SDL_SCANCODE_COMMA>();
      scancode_check<Scancode::Period, SDL_SCANCODE_PERIOD>();
      scancode_check<Scancode::Slash, SDL_SCANCODE_SLASH>();
      scancode_check<Scancode::CapsLock, SDL_SCANCODE_CAPSLOCK>();
      scancode_check<Scancode::F1, SDL_SCANCODE_F1>();
      scancode_check<Scancode::F2, SDL_SCANCODE_F2>();
      scancode_check<Scancode::F3, SDL_SCANCODE_F3>();
      scancode_check<Scancode::F4, SDL_SCANCODE_F4>();
      scancode_check<Scancode::F5, SDL_SCANCODE_F5>();
      scancode_check<Scancode::F6, SDL_SCANCODE_F6>();
      scancode_check<Scancode::F7, SDL_SCANCODE_F7>();
      scancode_check<Scancode::F8, SDL_SCANCODE_F8>();
      scancode_check<Scancode::F9, SDL_SCANCODE_F9>();
      scancode_check<Scancode::F10, SDL_SCANCODE_F10>();
      scancode_check<Scancode::F11, SDL_SCANCODE_F11>();
      scancode_check<Scancode::F12, SDL_SCANCODE_F12>();
      scancode_check<Scancode::PrintScreen, SDL_SCANCODE_PRINTSCREEN>();
      scancode_check<Scancode::ScrollLock, SDL_SCANCODE_SCROLLLOCK>();
      scancode_check<Scancode::Pause, SDL_SCANCODE_PAUSE>();
      scancode_check<Scancode::Insert, SDL_SCANCODE_INSERT>();
      scancode_check<Scancode::Home, SDL_SCANCODE_HOME>();
      scancode_check<Scancode::PageUp, SDL_SCANCODE_PAGEUP>();
      scancode_check<Scancode::Delete, SDL_SCANCODE_DELETE>();
      scancode_check<Scancode::End, SDL_SCANCODE_END>();
      scancode_check<Scancode::PageDown, SDL_SCANCODE_PAGEDOWN>();
      scancode_check<Scancode::Right, SDL_SCANCODE_RIGHT>();
      scancode_check<Scancode::Left, SDL_SCANCODE_LEFT>();
      scancode_check<Scancode::Down, SDL_SCANCODE_DOWN>();
      scancode_check<Scancode::Up, SDL_SCANCODE_UP>();
      scancode_check<Scancode::NumLockClear, SDL_SCANCODE_NUMLOCKCLEAR>();
      scancode_check<Scancode::NumpadDivide, SDL_SCANCODE_KP_DIVIDE>();
      scancode_check<Scancode::NumpadMultiply, SDL_SCANCODE_KP_MULTIPLY>();
      scancode_check<Scancode::NumpadMinus, SDL_SCANCODE_KP_MINUS>();
      scancode_check<Scancode::NumpadPlus, SDL_SCANCODE_KP_PLUS>();
      scancode_check<Scancode::NumpadEnter, SDL_SCANCODE_KP_ENTER>();
      scancode_check<Scancode::Numpad1, SDL_SCANCODE_KP_1>();
      scancode_check<Scancode::Numpad2, SDL_SCANCODE_KP_2>();
      scancode_check<Scancode::Numpad3, SDL_SCANCODE_KP_3>();
      scancode_check<Scancode::Numpad4, SDL_SCANCODE_KP_4>();
      scancode_check<Scancode::Numpad5, SDL_SCANCODE_KP_5>();
      scancode_check<Scancode::Numpad6, SDL_SCANCODE_KP_6>();
      scancode_check<Scancode::Numpad7, SDL_SCANCODE_KP_7>();
      scancode_check<Scancode::Numpad8, SDL_SCANCODE_KP_8>();
      scancode_check<Scancode::Numpad9, SDL_SCANCODE_KP_9>();
      scancode_check<Scancode::Numpad0, SDL_SCANCODE_KP_0>();
      scancode_check<Scancode::NumpadPeriod, SDL_SCANCODE_KP_PERIOD>();
      scancode_check<Scancode::NonUsBackslash, SDL_SCANCODE_NONUSBACKSLASH>();
      scancode_check<Scancode::Application, SDL_SCANCODE_APPLICATION>();
      scancode_check<Scancode::Power, SDL_SCANCODE_POWER>();
      scancode_check<Scancode::NumpadEquals, SDL_SCANCODE_KP_EQUALS>();
      scancode_check<Scancode::F13, SDL_SCANCODE_F13>();
      scancode_check<Scancode::F14, SDL_SCANCODE_F14>();
      scancode_check<Scancode::F15, SDL_SCANCODE_F15>();
      scancode_check<Scancode::F16, SDL_SCANCODE_F16>();
      scancode_check<Scancode::F17, SDL_SCANCODE_F17>();
      scancode_check<Scancode::F18, SDL_SCANCODE_F18>();
      scancode_check<Scancode::F19, SDL_SCANCODE_F19>();
      scancode_check<Scancode::F20, SDL_SCANCODE_F20>();
      scancode_check<Scancode::F21, SDL_SCANCODE_F21>();
      scancode_check<Scancode::F22, SDL_SCANCODE_F22>();
      scancode_check<Scancode::F23, SDL_SCANCODE_F23>();
      scancode_check<Scancode::F24, SDL_SCANCODE_F24>();
      scancode_check<Scancode::Execute, SDL_SCANCODE_EXECUTE>();
      scancode_check<Scancode::Help, SDL_SCANCODE_HELP>();
      scancode_check<Scancode::Menu, SDL_SCANCODE_MENU>();
      scancode_check<Scancode::Select, SDL_SCANCODE_SELECT>();
      scancode_check<Scancode::Stop, SDL_SCANCODE_STOP>();
      scancode_check<Scancode::Again, SDL_SCANCODE_AGAIN>();
      scancode_check<Scancode::Undo, SDL_SCANCODE_UNDO>();
      scancode_check<Scancode::Cut, SDL_SCANCODE_CUT>();
      scancode_check<Scancode::Copy, SDL_SCANCODE_COPY>();
      scancode_check<Scancode::Paste, SDL_SCANCODE_PASTE>();
      scancode_check<Scancode::Find, SDL_SCANCODE_FIND>();
      scancode_check<Scancode::Mute, SDL_SCANCODE_MUTE>();
      scancode_check<Scancode::VolumeUp, SDL_SCANCODE_VOLUMEUP>();
      scancode_check<Scancode::VolumeDown, SDL_SCANCODE_VOLUMEDOWN>();
      scancode_check<Scancode::NumpadComma, SDL_SCANCODE_KP_COMMA>();
      scancode_check<Scancode::NumpadEqualsAs400, SDL_SCANCODE_KP_EQUALSAS400>();
      scancode_check<Scancode::International1, SDL_SCANCODE_INTERNATIONAL1>();
      scancode_check<Scancode::International2, SDL_SCANCODE_INTERNATIONAL2>();
      scancode_check<Scancode::International3, SDL_SCANCODE_INTERNATIONAL3>();
      scancode_check<Scancode::International4, SDL_SCANCODE_INTERNATIONAL4>();
      scancode_check<Scancode::International5, SDL_SCANCODE_INTERNATIONAL5>();
      scancode_check<Scancode::International6, SDL_SCANCODE_INTERNATIONAL6>();
      scancode_check<Scancode::International7, SDL_SCANCODE_INTERNATIONAL7>();
      scancode_check<Scancode::International8, SDL_SCANCODE_INTERNATIONAL8>();
      scancode_check<Scancode::International9, SDL_SCANCODE_INTERNATIONAL9>();
      scancode_check<Scancode::Lang1, SDL_SCANCODE_LANG1>();
      scancode_check<Scancode::Lang2, SDL_SCANCODE_LANG2>();
      scancode_check<Scancode::Lang3, SDL_SCANCODE_LANG3>();
      scancode_check<Scancode::Lang4, SDL_SCANCODE_LANG4>();
      scancode_check<Scancode::Lang5, SDL_SCANCODE_LANG5>();
      scancode_check<Scancode::Lang6, SDL_SCANCODE_LANG6>();
      scancode_check<Scancode::Lang7, SDL_SCANCODE_LANG7>();
      scancode_check<Scancode::Lang8, SDL_SCANCODE_LANG8>();
      scancode_check<Scancode::Lang9, SDL_SCANCODE_LANG9>();
      scancode_check<Scancode::AltErase, SDL_SCANCODE_ALTERASE>();
      scancode_check<Scancode::SysReq, SDL_SCANCODE_SYSREQ>();
      scancode_check<Scancode::Cancel, SDL_SCANCODE_CANCEL>();
      scancode_check<Scancode::Prior, SDL_SCANCODE_PRIOR>();
      scancode_check<Scancode::Return2, SDL_SCANCODE_RETURN2>();
      scancode_check<Scancode::Separator, SDL_SCANCODE_SEPARATOR>();
      scancode_check<Scancode::Out, SDL_SCANCODE_OUT>();
      scancode_check<Scancode::Oper, SDL_SCANCODE_OPER>();
      scancode_check<Scancode::ClearAgain, SDL_SCANCODE_CLEARAGAIN>();
      scancode_check<Scancode::CrSel, SDL_SCANCODE_CRSEL>();
      scancode_check<Scancode::ExSel, SDL_SCANCODE_EXSEL>();
      scancode_check<Scancode::Numpad00, SDL_SCANCODE_KP_00>();
      scancode_check<Scancode::Numpad000, SDL_SCANCODE_KP_000>();
      scancode_check<Scancode::ThousandsSeparator, SDL_SCANCODE_THOUSANDSSEPARATOR>();
      scancode_check<Scancode::DecimalSeparator, SDL_SCANCODE_DECIMALSEPARATOR>();
      scancode_check<Scancode::CurrencyUnit, SDL_SCANCODE_CURRENCYUNIT>();
      scancode_check<Scancode::CurrencySubUnit, SDL_SCANCODE_CURRENCYSUBUNIT>();
      scancode_check<Scancode::NumpadLeftParen, SDL_SCANCODE_KP_LEFTPAREN>();
      scancode_check<Scancode::NumpadRightParen, SDL_SCANCODE_KP_RIGHTPAREN>();
      scancode_check<Scancode::NumpadLeftBrace, SDL_SCANCODE_KP_LEFTBRACE>();
      scancode_check<Scancode::NumpadRightBrace, SDL_SCANCODE_KP_RIGHTBRACE>();
      scancode_check<Scancode::NumpadTab, SDL_SCANCODE_KP_TAB>();
      scancode_check<Scancode::NumpadBackspace, SDL_SCANCODE_KP_BACKSPACE>();
      scancode_check<Scancode::NumpadA, SDL_SCANCODE_KP_A>();
      scancode_check<Scancode::NumpadB, SDL_SCANCODE_KP_B>();
      scancode_check<Scancode::NumpadC, SDL_SCANCODE_KP_C>();
      scancode_check<Scancode::NumpadD, SDL_SCANCODE_KP_D>();
      scancode_check<Scancode::NumpadE, SDL_SCANCODE_KP_E>();
      scancode_check<Scancode::NumpadF, SDL_SCANCODE_KP_F>();
      scancode_check<Scancode::NumpadXor, SDL_SCANCODE_KP_XOR>();
      scancode_check<Scancode::NumpadPower, SDL_SCANCODE_KP_POWER>();
      scancode_check<Scancode::NumpadPercent, SDL_SCANCODE_KP_PERCENT>();
      scancode_check<Scancode::NumpadLess, SDL_SCANCODE_KP_LESS>();
      scancode_check<Scancode::NumpadGreater, SDL_SCANCODE_KP_GREATER>();
      scancode_check<Scancode::NumpadAmpersand, SDL_SCANCODE_KP_AMPERSAND>();
      scancode_check<Scancode::NumpadDblAmpersand, SDL_SCANCODE_KP_DBLAMPERSAND>();
      scancode_check<Scancode::NumpadVerticalBar, SDL_SCANCODE_KP_VERTICALBAR>();
      scancode_check<Scancode::NumpadDblVerticalBar, SDL_SCANCODE_KP_DBLVERTICALBAR>();
      scancode_check<Scancode::NumpadColon, SDL_SCANCODE_KP_COLON>();
      scancode_check<Scancode::NumpadHash, SDL_SCANCODE_KP_HASH>();
      scancode_check<Scancode::NumpadSpace, SDL_SCANCODE_KP_SPACE>();
      scancode_check<Scancode::NumpadAt, SDL_SCANCODE_KP_AT>();
      scancode_check<Scancode::NumpadExclam, SDL_SCANCODE_KP_EXCLAM>();
      scancode_check<Scancode::NumpadMemStore, SDL_SCANCODE_KP_MEMSTORE>();
      scancode_check<Scancode::NumpadMemRecall, SDL_SCANCODE_KP_MEMRECALL>();
      scancode_check<Scancode::NumpadMemClear, SDL_SCANCODE_KP_MEMCLEAR>();
      scancode_check<Scancode::NumpadMemAdd, SDL_SCANCODE_KP_MEMADD>();
      scancode_check<Scancode::NumpadMemSubtract, SDL_SCANCODE_KP_MEMSUBTRACT>();
      scancode_check<Scancode::NumpadMemMultiply, SDL_SCANCODE_KP_MEMMULTIPLY>();
      scancode_check<Scancode::NumpadMemDivide, SDL_SCANCODE_KP_MEMDIVIDE>();
      scancode_check<Scancode::NumpadPlusMinus, SDL_SCANCODE_KP_PLUSMINUS>();
      scancode_check<Scancode::NumpadClear, SDL_SCANCODE_KP_CLEAR>();
      scancode_check<Scancode::NumpadClearEntry, SDL_SCANCODE_KP_CLEARENTRY>();
      scancode_check<Scancode::NumpadBinary, SDL_SCANCODE_KP_BINARY>();
      scancode_check<Scancode::NumpadOctal, SDL_SCANCODE_KP_OCTAL>();
      scancode_check<Scancode::NumpadDecimal, SDL_SCANCODE_KP_DECIMAL>();
      scancode_check<Scancode::NumpadHexadecimal, SDL_SCANCODE_KP_HEXADECIMAL>();
      scancode_check<Scancode::LeftCtrl, SDL_SCANCODE_LCTRL>();
      scancode_check<Scancode::LeftShift, SDL_SCANCODE_LSHIFT>();
      scancode_check<Scancode::LeftAlt, SDL_SCANCODE_LALT>();
      scancode_check<Scancode::LeftGui, SDL_SCANCODE_LGUI>();
      scancode_check<Scancode::RightCtrl, SDL_SCANCODE_RCTRL>();
      scancode_check<Scancode::RightShift, SDL_SCANCODE_RSHIFT>();
      scancode_check<Scancode::RightAlt, SDL_SCANCODE_RALT>();
      scancode_check<Scancode::RightGui, SDL_SCANCODE_RGUI>();
      scancode_check<Scancode::Mode, SDL_SCANCODE_MODE>();

      keycode_check<Keycode::Unknown, SDLK_UNKNOWN>();
      keycode_check<Keycode::Return, SDLK_RETURN>();
      keycode_check<Keycode::Escape, SDLK_ESCAPE>();
      keycode_check<Keycode::Backspace, SDLK_BACKSPACE>();
      keycode_check<Keycode::Tab, SDLK_TAB>();
      keycode_check<Keycode::Space, SDLK_SPACE>();
      keycode_check<Keycode::Exclaim, SDLK_EXCLAIM>();
      keycode_check<Keycode::QuoteDbl, SDLK_QUOTEDBL>();
      keycode_check<Keycode::Hash, SDLK_HASH>();
      keycode_check<Keycode::Percent, SDLK_PERCENT>();
      keycode_check<Keycode::Dollar, SDLK_DOLLAR>();
      keycode_check<Keycode::Ampersand, SDLK_AMPERSAND>();
      keycode_check<Keycode::Quote, SDLK_QUOTE>();
      keycode_check<Keycode::LeftParen, SDLK_LEFTPAREN>();
      keycode_check<Keycode::RightParen, SDLK_RIGHTPAREN>();
      keycode_check<Keycode::Asterisk, SDLK_ASTERISK>();
      keycode_check<Keycode::Plus, SDLK_PLUS>();
      keycode_check<Keycode::Comma, SDLK_COMMA>();
      keycode_check<Keycode::Minus, SDLK_MINUS>();
      keycode_check<Keycode::Period, SDLK_PERIOD>();
      keycode_check<Keycode::Slash, SDLK_SLASH>();
      keycode_check<Keycode::Num1, SDLK_1>();
      keycode_check<Keycode::Num2, SDLK_2>();
      keycode_check<Keycode::Num3, SDLK_3>();
      keycode_check<Keycode::Num4, SDLK_4>();
      keycode_check<Keycode::Num5, SDLK_5>();
      keycode_check<Keycode::Num6, SDLK_6>();
      keycode_check<Keycode::Num7, SDLK_7>();
      keycode_check<Keycode::Num8, SDLK_8>();
      keycode_check<Keycode::Num9, SDLK_9>();
      keycode_check<Keycode::Num0, SDLK_0>();
      keycode_check<Keycode::Colon, SDLK_COLON>();
      keycode_check<Keycode::SemiColon, SDLK_SEMICOLON>();
      keycode_check<Keycode::Less, SDLK_LESS>();
      keycode_check<Keycode::Equals, SDLK_EQUALS>();
      keycode_check<Keycode::Greater, SDLK_GREATER>();
      keycode_check<Keycode::Question, SDLK_QUESTION>();
      keycode_check<Keycode::At, SDLK_AT>();
      keycode_check<Keycode::LeftBracket, SDLK_LEFTBRACKET>();
      keycode_check<Keycode::Backslash, SDLK_BACKSLASH>();
      keycode_check<Keycode::RightBracket, SDLK_RIGHTBRACKET>();
      keycode_check<Keycode::Caret, SDLK_CARET>();
      keycode_check<Keycode::Underscore, SDLK_UNDERSCORE>();
      keycode_check<Keycode::Backquote, SDLK_BACKQUOTE>();
      keycode_check<Keycode::A, SDLK_a>();
      keycode_check<Keycode::B, SDLK_b>();
      keycode_check<Keycode::C, SDLK_c>();
      keycode_check<Keycode::D, SDLK_d>();
      keycode_check<Keycode::E, SDLK_e>();
      keycode_check<Keycode::F, SDLK_f>();
      keycode_check<Keycode::G, SDLK_g>();
      keycode_check<Keycode::H, SDLK_h>();
      keycode_check<Keycode::I, SDLK_i>();
      keycode_check<Keycode::J, SDLK_j>();
      keycode_check<Keycode::K, SDLK_k>();
      keycode_check<Keycode::L, SDLK_l>();
      keycode_check<Keycode::M, SDLK_m>();
      keycode_check<Keycode::N, SDLK_n>();
      keycode_check<Keycode::O, SDLK_o>();
      keycode_check<Keycode::P, SDLK_p>();
      keycode_check<Keycode::Q, SDLK_q>();
      keycode_check<Keycode::R, SDLK_r>();
      keycode_check<Keycode::S, SDLK_s>();
      keycode_check<Keycode::T, SDLK_t>();
      keycode_check<Keycode::U, SDLK_u>();
      keycode_check<Keycode::V, SDLK_v>();
      keycode_check<Keycode::W, SDLK_w>();
      keycode_check<Keycode::X, SDLK_x>();
      keycode_check<Keycode::Y, SDLK_y>();
      keycode_check<Keycode::Z, SDLK_z>();
      keycode_check<Keycode::CapsLock, SDLK_CAPSLOCK>();
      keycode_check<Keycode::F1, SDLK_F1>();
      keycode_check<Keycode::F2, SDLK_F2>();
      keycode_check<Keycode::F3, SDLK_F3>();
      keycode_check<Keycode::F4, SDLK_F4>();
      keycode_check<Keycode::F5, SDLK_F5>();
      keycode_check<Keycode::F6, SDLK_F6>();
      keycode_check<Keycode::F7, SDLK_F7>();
      keycode_check<Keycode::F8, SDLK_F8>();
      keycode_check<Keycode::F9, SDLK_F9>();
      keycode_check<Keycode::F10, SDLK_F10>();
      keycode_check<Keycode::F11, SDLK_F11>();
      keycode_check<Keycode::F12, SDLK_F12>();
      keycode_check<Keycode::PrintScreen, SDLK_PRINTSCREEN>();
      keycode_check<Keycode::ScrollLock, SDLK_SCROLLLOCK>();
      keycode_check<Keycode::Pause, SDLK_PAUSE>();
      keycode_check<Keycode::Insert, SDLK_INSERT>();
      keycode_check<Keycode::Home, SDLK_HOME>();
      keycode_check<Keycode::PageUp, SDLK_PAGEUP>();
      keycode_check<Keycode::Delete, SDLK_DELETE>();
      keycode_check<Keycode::End, SDLK_END>();
      keycode_check<Keycode::PageDown, SDLK_PAGEDOWN>();
      keycode_check<Keycode::Right, SDLK_RIGHT>();
      keycode_check<Keycode::Left, SDLK_LEFT>();
      keycode_check<Keycode::Down, SDLK_DOWN>();
      keycode_check<Keycode::Up, SDLK_UP>();
      keycode_check<Keycode::NumLockClear, SDLK_NUMLOCKCLEAR>();
      keycode_check<Keycode::NumpadDivide, SDLK_KP_DIVIDE>();
      keycode_check<Keycode::NumpadMultiply, SDLK_KP_MULTIPLY>();
      keycode_check<Keycode::NumpadMinus, SDLK_KP_MINUS>();
      keycode_check<Keycode::NumpadPlus, SDLK_KP_PLUS>();
      keycode_check<Keycode::NumpadEnter, SDLK_KP_ENTER>();
      keycode_check<Keycode::Numpad1, SDLK_KP_1>();
      keycode_check<Keycode::Numpad2, SDLK_KP_2>();
      keycode_check<Keycode::Numpad3, SDLK_KP_3>();
      keycode_check<Keycode::Numpad4, SDLK_KP_4>();
      keycode_check<Keycode::Numpad5, SDLK_KP_5>();
      keycode_check<Keycode::Numpad6, SDLK_KP_6>();
      keycode_check<Keycode::Numpad7, SDLK_KP_7>();
      keycode_check<Keycode::Numpad8, SDLK_KP_8>();
      keycode_check<Keycode::Numpad9, SDLK_KP_9>();
      keycode_check<Keycode::Numpad0, SDLK_KP_0>();
      keycode_check<Keycode::NumpadPeriod, SDLK_KP_PERIOD>();
      keycode_check<Keycode::Application, SDLK_APPLICATION>();
      keycode_check<Keycode::Power, SDLK_POWER>();
      keycode_check<Keycode::NumpadEquals, SDLK_KP_EQUALS>();
      keycode_check<Keycode::F13, SDLK_F13>();
      keycode_check<Keycode::F14, SDLK_F14>();
      keycode_check<Keycode::F15, SDLK_F15>();
      keycode_check<Keycode::F16, SDLK_F16>();
      keycode_check<Keycode::F17, SDLK_F17>();
      keycode_check<Keycode::F18, SDLK_F18>();
      keycode_check<Keycode::F19, SDLK_F19>();
      keycode_check<Keycode::F20, SDLK_F20>();
      keycode_check<Keycode::F21, SDLK_F21>();
      keycode_check<Keycode::F22, SDLK_F22>();
      keycode_check<Keycode::F23, SDLK_F23>();
      keycode_check<Keycode::F24, SDLK_F24>();
      keycode_check<Keycode::Execute, SDLK_EXECUTE>();
      keycode_check<Keycode::Help, SDLK_HELP>();
      keycode_check<Keycode::Menu, SDLK_MENU>();
      keycode_check<Keycode::Select, SDLK_SELECT>();
      keycode_check<Keycode::Stop, SDLK_STOP>();
      keycode_check<Keycode::Again, SDLK_AGAIN>();
      keycode_check<Keycode::Undo, SDLK_UNDO>();
      keycode_check<Keycode::Cut, SDLK_CUT>();
      keycode_check<Keycode::Copy, SDLK_COPY>();
      keycode_check<Keycode::Paste, SDLK_PASTE>();
      keycode_check<Keycode::Find, SDLK_FIND>();
      keycode_check<Keycode::Mute, SDLK_MUTE>();
      keycode_check<Keycode::VolumeUp, SDLK_VOLUMEUP>();
      keycode_check<Keycode::VolumeDown, SDLK_VOLUMEDOWN>();
      keycode_check<Keycode::NumpadComma, SDLK_KP_COMMA>();
      keycode_check<Keycode::NumpadEqualsAs400, SDLK_KP_EQUALSAS400>();
      keycode_check<Keycode::AltErase, SDLK_ALTERASE>();
      keycode_check<Keycode::SysReq, SDLK_SYSREQ>();
      keycode_check<Keycode::Cancel, SDLK_CANCEL>();
      keycode_check<Keycode::Prior, SDLK_PRIOR>();
      keycode_check<Keycode::Return2, SDLK_RETURN2>();
      keycode_check<Keycode::Separator, SDLK_SEPARATOR>();
      keycode_check<Keycode::Out, SDLK_OUT>();
      keycode_check<Keycode::Oper, SDLK_OPER>();
      keycode_check<Keycode::ClearAgain, SDLK_CLEARAGAIN>();
      keycode_check<Keycode::CrSel, SDLK_CRSEL>();
      keycode_check<Keycode::ExSel, SDLK_EXSEL>();
      keycode_check<Keycode::Numpad00, SDLK_KP_00>();
      keycode_check<Keycode::Numpad000, SDLK_KP_000>();
      keycode_check<Keycode::ThousandsSeparator, SDLK_THOUSANDSSEPARATOR>();
      keycode_check<Keycode::DecimalSeparator, SDLK_DECIMALSEPARATOR>();
      keycode_check<Keycode::CurrencyUnit, SDLK_CURRENCYUNIT>();
      keycode_check<Keycode::CurrencySubUnit, SDLK_CURRENCYSUBUNIT>();
      keycode_check<Keycode::NumpadLeftParen, SDLK_KP_LEFTPAREN>();
      keycode_check<Keycode::NumpadRightParen, SDLK_KP_RIGHTPAREN>();
      keycode_check<Keycode::NumpadLeftBrace, SDLK_KP_LEFTBRACE>();
      keycode_check<Keycode::NumpadRightBrace, SDLK_KP_RIGHTBRACE>();
      keycode_check<Keycode::NumpadTab, SDLK_KP_TAB>();
      keycode_check<Keycode::NumpadBackspace, SDLK_KP_BACKSPACE>();
      keycode_check<Keycode::NumpadA, SDLK_KP_A>();
      keycode_check<Keycode::NumpadB, SDLK_KP_B>();
      keycode_check<Keycode::NumpadC, SDLK_KP_C>();
      keycode_check<Keycode::NumpadD, SDLK_KP_D>();
      keycode_check<Keycode::NumpadE, SDLK_KP_E>();
      keycode_check<Keycode::NumpadF, SDLK_KP_F>();
      keycode_check<Keycode::NumpadXor, SDLK_KP_XOR>();
      keycode_check<Keycode::NumpadPower, SDLK_KP_POWER>();
      keycode_check<Keycode::NumpadPercent, SDLK_KP_PERCENT>();
      keycode_check<Keycode::NumpadLess, SDLK_KP_LESS>();
      keycode_check<Keycode::NumpadGreater, SDLK_KP_GREATER>();
      keycode_check<Keycode::NumpadAmpersand, SDLK_KP_AMPERSAND>();
      keycode_check<Keycode::NumpadDblAmpersand, SDLK_KP_DBLAMPERSAND>();
      keycode_check<Keycode::NumpadVerticalBar, SDLK_KP_VERTICALBAR>();
      keycode_check<Keycode::NumpadDblVerticalBar, SDLK_KP_DBLVERTICALBAR>();
      keycode_check<Keycode::NumpadColon, SDLK_KP_COLON>();
      keycode_check<Keycode::NumpadHash, SDLK_KP_HASH>();
      keycode_check<Keycode::NumpadSpace, SDLK_KP_SPACE>();
      keycode_check<Keycode::NumpadAt, SDLK_KP_AT>();
      keycode_check<Keycode::NumpadExclam, SDLK_KP_EXCLAM>();
      keycode_check<Keycode::NumpadMemStore, SDLK_KP_MEMSTORE>();
      keycode_check<Keycode::NumpadMemRecall, SDLK_KP_MEMRECALL>();
      keycode_check<Keycode::NumpadMemClear, SDLK_KP_MEMCLEAR>();
      keycode_check<Keycode::NumpadMemAdd, SDLK_KP_MEMADD>();
      keycode_check<Keycode::NumpadMemSubtract, SDLK_KP_MEMSUBTRACT>();
      keycode_check<Keycode::NumpadMemMultiply, SDLK_KP_MEMMULTIPLY>();
      keycode_check<Keycode::NumpadMemDivide, SDLK_KP_MEMDIVIDE>();
      keycode_check<Keycode::NumpadPlusMinus, SDLK_KP_PLUSMINUS>();
      keycode_check<Keycode::NumpadClear, SDLK_KP_CLEAR>();
      keycode_check<Keycode::NumpadClearEntry, SDLK_KP_CLEARENTRY>();
      keycode_check<Keycode::NumpadBinary, SDLK_KP_BINARY>();
      keycode_check<Keycode::NumpadOctal, SDLK_KP_OCTAL>();
      keycode_check<Keycode::NumpadDecimal, SDLK_KP_DECIMAL>();
      keycode_check<Keycode::NumpadHexadecimal, SDLK_KP_HEXADECIMAL>();
      keycode_check<Keycode::LeftCtrl, SDLK_LCTRL>();
      keycode_check<Keycode::LeftShift, SDLK_LSHIFT>();
      keycode_check<Keycode::LeftAlt, SDLK_LALT>();
      keycode_check<Keycode::LeftGui, SDLK_LGUI>();
      keycode_check<Keycode::RightCtrl, SDLK_RCTRL>();
      keycode_check<Keycode::RightShift, SDLK_RSHIFT>();
      keycode_check<Keycode::RightAlt, SDLK_RALT>();
      keycode_check<Keycode::RightGui, SDLK_RGUI>();
      keycode_check<Keycode::Mode, SDLK_MODE>();
    }

  } // namespace

  const char* Keyboard::scancode_name(Scancode scancode)
  {
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(scancode));
  }

  Scancode Keyboard::scancode_from_name(const char* name)
  {
    return static_cast<Scancode>(SDL_GetScancodeFromName(name));
  }

  const char* Keyboard::keycode_name(Keycode keycode)
  {
    return SDL_GetKeyName(static_cast<SDL_Keycode>(keycode));
  }

  Keycode Keyboard::keycode_from_name(const char* name)
  {
    return static_cast<Keycode>(SDL_GetKeyFromName(name));
  }

  Keycode Keyboard::localize(Scancode scancode)
  {
    return static_cast<Keycode>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(scancode)));
  }

  Scancode Keyboard::unlocalize(Keycode keycode)
  {
    return static_cast<Scancode>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode)));
  }

} // namespace gf
