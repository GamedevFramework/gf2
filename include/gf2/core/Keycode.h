// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CORE_KEYCODE_H
#define GF_CORE_KEYCODE_H

#include <cstdint>

#include "Scancode.h"

namespace gf {

  namespace details {
    constexpr uint32_t to_keycode(Scancode scancode)
    {
      return static_cast<uint32_t>(scancode) | (UINT32_C(1) << 30);
    }
  } // namespace details

  enum class Keycode : uint32_t {
    Unknown = 0,
    Return = '\r',
    Escape = '\033',
    Backspace = '\b',
    Tab = '\t',
    Space = ' ',
    Exclaim = '!',
    DoubleApostrophe = '"',
    Hash = '#',
    Percent = '%',
    Dollar = '$',
    Ampersand = '&',
    Quote = '\'',
    LeftParen = '(',
    RightParen = ')',
    Asterisk = '*',
    Plus = '+',
    Comma = ',',
    Minus = '-',
    Period = '.',
    Slash = '/',
    Num0 = '0',
    Num1 = '1',
    Num2 = '2',
    Num3 = '3',
    Num4 = '4',
    Num5 = '5',
    Num6 = '6',
    Num7 = '7',
    Num8 = '8',
    Num9 = '9',
    Colon = ':',
    SemiColon = ';',
    Less = '<',
    Equals = '=',
    Greater = '>',
    Question = '?',
    At = '@',
    LeftBracket = '[',
    Backslash = '\\',
    RightBracket = ']',
    Caret = '^',
    Underscore = '_',
    Grave = '`',
    A = 'a',
    B = 'b',
    C = 'c',
    D = 'd',
    E = 'e',
    F = 'f',
    G = 'g',
    H = 'h',
    I = 'i',
    J = 'j',
    K = 'k',
    L = 'l',
    M = 'm',
    N = 'n',
    O = 'o',
    P = 'p',
    Q = 'q',
    R = 'r',
    S = 's',
    T = 't',
    U = 'u',
    V = 'v',
    W = 'w',
    X = 'x',
    Y = 'y',
    Z = 'z',
    CapsLock = details::to_keycode(Scancode::CapsLock),
    F1 = details::to_keycode(Scancode::F1),
    F2 = details::to_keycode(Scancode::F2),
    F3 = details::to_keycode(Scancode::F3),
    F4 = details::to_keycode(Scancode::F4),
    F5 = details::to_keycode(Scancode::F5),
    F6 = details::to_keycode(Scancode::F6),
    F7 = details::to_keycode(Scancode::F7),
    F8 = details::to_keycode(Scancode::F8),
    F9 = details::to_keycode(Scancode::F9),
    F10 = details::to_keycode(Scancode::F10),
    F11 = details::to_keycode(Scancode::F11),
    F12 = details::to_keycode(Scancode::F12),
    PrintScreen = details::to_keycode(Scancode::PrintScreen),
    ScrollLock = details::to_keycode(Scancode::ScrollLock),
    Pause = details::to_keycode(Scancode::Pause),
    Insert = details::to_keycode(Scancode::Insert),
    Home = details::to_keycode(Scancode::Home),
    PageUp = details::to_keycode(Scancode::PageUp),
    Delete = '\177',
    End = details::to_keycode(Scancode::End),
    PageDown = details::to_keycode(Scancode::PageDown),
    Right = details::to_keycode(Scancode::Right),
    Left = details::to_keycode(Scancode::Left),
    Down = details::to_keycode(Scancode::Down),
    Up = details::to_keycode(Scancode::Up),
    NumLockClear = details::to_keycode(Scancode::NumLockClear),
    NumpadDivide = details::to_keycode(Scancode::NumpadDivide),
    NumpadMultiply = details::to_keycode(Scancode::NumpadMultiply),
    NumpadMinus = details::to_keycode(Scancode::NumpadMinus),
    NumpadPlus = details::to_keycode(Scancode::NumpadPlus),
    NumpadEnter = details::to_keycode(Scancode::NumpadEnter),
    Numpad1 = details::to_keycode(Scancode::Numpad1),
    Numpad2 = details::to_keycode(Scancode::Numpad2),
    Numpad3 = details::to_keycode(Scancode::Numpad3),
    Numpad4 = details::to_keycode(Scancode::Numpad4),
    Numpad5 = details::to_keycode(Scancode::Numpad5),
    Numpad6 = details::to_keycode(Scancode::Numpad6),
    Numpad7 = details::to_keycode(Scancode::Numpad7),
    Numpad8 = details::to_keycode(Scancode::Numpad8),
    Numpad9 = details::to_keycode(Scancode::Numpad9),
    Numpad0 = details::to_keycode(Scancode::Numpad0),
    NumpadPeriod = details::to_keycode(Scancode::NumpadPeriod),
    Application = details::to_keycode(Scancode::Application),
    Power = details::to_keycode(Scancode::Power),
    NumpadEquals = details::to_keycode(Scancode::NumpadEquals),
    F13 = details::to_keycode(Scancode::F13),
    F14 = details::to_keycode(Scancode::F14),
    F15 = details::to_keycode(Scancode::F15),
    F16 = details::to_keycode(Scancode::F16),
    F17 = details::to_keycode(Scancode::F17),
    F18 = details::to_keycode(Scancode::F18),
    F19 = details::to_keycode(Scancode::F19),
    F20 = details::to_keycode(Scancode::F20),
    F21 = details::to_keycode(Scancode::F21),
    F22 = details::to_keycode(Scancode::F22),
    F23 = details::to_keycode(Scancode::F23),
    F24 = details::to_keycode(Scancode::F24),
    Execute = details::to_keycode(Scancode::Execute),
    Help = details::to_keycode(Scancode::Help),
    Menu = details::to_keycode(Scancode::Menu),
    Select = details::to_keycode(Scancode::Select),
    Stop = details::to_keycode(Scancode::Stop),
    Again = details::to_keycode(Scancode::Again),
    Undo = details::to_keycode(Scancode::Undo),
    Cut = details::to_keycode(Scancode::Cut),
    Copy = details::to_keycode(Scancode::Copy),
    Paste = details::to_keycode(Scancode::Paste),
    Find = details::to_keycode(Scancode::Find),
    Mute = details::to_keycode(Scancode::Mute),
    VolumeUp = details::to_keycode(Scancode::VolumeUp),
    VolumeDown = details::to_keycode(Scancode::VolumeDown),
    NumpadComma = details::to_keycode(Scancode::NumpadComma),
    NumpadEqualsAs400 = details::to_keycode(Scancode::NumpadEqualsAs400),
    AltErase = details::to_keycode(Scancode::AltErase),
    SysReq = details::to_keycode(Scancode::SysReq),
    Cancel = details::to_keycode(Scancode::Cancel),
    Clear = details::to_keycode(Scancode::Clear),
    Prior = details::to_keycode(Scancode::Prior),
    Return2 = details::to_keycode(Scancode::Return2),
    Separator = details::to_keycode(Scancode::Separator),
    Out = details::to_keycode(Scancode::Out),
    Oper = details::to_keycode(Scancode::Oper),
    ClearAgain = details::to_keycode(Scancode::ClearAgain),
    CrSel = details::to_keycode(Scancode::CrSel),
    ExSel = details::to_keycode(Scancode::ExSel),
    Numpad00 = details::to_keycode(Scancode::Numpad00),
    Numpad000 = details::to_keycode(Scancode::Numpad000),
    ThousandsSeparator = details::to_keycode(Scancode::ThousandsSeparator),
    DecimalSeparator = details::to_keycode(Scancode::DecimalSeparator),
    CurrencyUnit = details::to_keycode(Scancode::CurrencyUnit),
    CurrencySubUnit = details::to_keycode(Scancode::CurrencySubUnit),
    NumpadLeftParen = details::to_keycode(Scancode::NumpadLeftParen),
    NumpadRightParen = details::to_keycode(Scancode::NumpadRightParen),
    NumpadLeftBrace = details::to_keycode(Scancode::NumpadLeftBrace),
    NumpadRightBrace = details::to_keycode(Scancode::NumpadRightBrace),
    NumpadTab = details::to_keycode(Scancode::NumpadTab),
    NumpadBackspace = details::to_keycode(Scancode::NumpadBackspace),
    NumpadA = details::to_keycode(Scancode::NumpadA),
    NumpadB = details::to_keycode(Scancode::NumpadB),
    NumpadC = details::to_keycode(Scancode::NumpadC),
    NumpadD = details::to_keycode(Scancode::NumpadD),
    NumpadE = details::to_keycode(Scancode::NumpadE),
    NumpadF = details::to_keycode(Scancode::NumpadF),
    NumpadXor = details::to_keycode(Scancode::NumpadXor),
    NumpadPower = details::to_keycode(Scancode::NumpadPower),
    NumpadPercent = details::to_keycode(Scancode::NumpadPercent),
    NumpadLess = details::to_keycode(Scancode::NumpadLess),
    NumpadGreater = details::to_keycode(Scancode::NumpadGreater),
    NumpadAmpersand = details::to_keycode(Scancode::NumpadAmpersand),
    NumpadDblAmpersand = details::to_keycode(Scancode::NumpadDblAmpersand),
    NumpadVerticalBar = details::to_keycode(Scancode::NumpadVerticalBar),
    NumpadDblVerticalBar = details::to_keycode(Scancode::NumpadDblVerticalBar),
    NumpadColon = details::to_keycode(Scancode::NumpadColon),
    NumpadHash = details::to_keycode(Scancode::NumpadHash),
    NumpadSpace = details::to_keycode(Scancode::NumpadSpace),
    NumpadAt = details::to_keycode(Scancode::NumpadAt),
    NumpadExclam = details::to_keycode(Scancode::NumpadExclam),
    NumpadMemStore = details::to_keycode(Scancode::NumpadMemStore),
    NumpadMemRecall = details::to_keycode(Scancode::NumpadMemRecall),
    NumpadMemClear = details::to_keycode(Scancode::NumpadMemClear),
    NumpadMemAdd = details::to_keycode(Scancode::NumpadMemAdd),
    NumpadMemSubtract = details::to_keycode(Scancode::NumpadMemSubtract),
    NumpadMemMultiply = details::to_keycode(Scancode::NumpadMemMultiply),
    NumpadMemDivide = details::to_keycode(Scancode::NumpadMemDivide),
    NumpadPlusMinus = details::to_keycode(Scancode::NumpadPlusMinus),
    NumpadClear = details::to_keycode(Scancode::NumpadClear),
    NumpadClearEntry = details::to_keycode(Scancode::NumpadClearEntry),
    NumpadBinary = details::to_keycode(Scancode::NumpadBinary),
    NumpadOctal = details::to_keycode(Scancode::NumpadOctal),
    NumpadDecimal = details::to_keycode(Scancode::NumpadDecimal),
    NumpadHexadecimal = details::to_keycode(Scancode::NumpadHexadecimal),
    LeftCtrl = details::to_keycode(Scancode::LeftCtrl),
    LeftShift = details::to_keycode(Scancode::LeftShift),
    LeftAlt = details::to_keycode(Scancode::LeftAlt),
    LeftGui = details::to_keycode(Scancode::LeftGui),
    RightCtrl = details::to_keycode(Scancode::RightCtrl),
    RightShift = details::to_keycode(Scancode::RightShift),
    RightAlt = details::to_keycode(Scancode::RightAlt),
    RightGui = details::to_keycode(Scancode::RightGui),
    Mode = details::to_keycode(Scancode::Mode),
  };

  constexpr Keycode AnyKeycode = Keycode{ std::numeric_limits<std::underlying_type_t<Keycode>>::max() };

}

#endif // GF_CORE_KEYCODE_H
