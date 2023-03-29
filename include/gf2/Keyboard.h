// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_KEYBOARD_H
#define GF_KEYBOARD_H

#include <cstdint>

#include "Flags.h"
#include "GraphicsApi.h"

namespace gf {

  enum class Modifier : uint32_t {
    None = 0x0000,
    LeftShift = 0x0001,
    RightShift = 0x0002,
    LeftControl = 0x0040,
    RightControl = 0x0080,
    LeftAlt = 0x0100,
    RightAlt = 0x0200,
    LeftGui = 0x0400,
    RightGui = 0x0800,
    Num = 0x1000,
    Caps = 0x2000,
    Mode = 0x4000,
    Shift = LeftShift | RightShift,
    Control = LeftControl | RightControl,
    Alt = LeftAlt | RightAlt,
    Gui = LeftGui | RightGui,
  };

  template<>
  struct EnableBitmaskOperators<Modifier> : std::true_type {
  };

  enum class Scancode : uint32_t {
    Unknown = 0,
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,
    Num1 = 30,
    Num2 = 31,
    Num3 = 32,
    Num4 = 33,
    Num5 = 34,
    Num6 = 35,
    Num7 = 36,
    Num8 = 37,
    Num9 = 38,
    Num0 = 39,
    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,
    Minus = 45,
    Equals = 46,
    LeftBracket = 47,
    RightBracket = 48,
    Backslash = 49,
    NonUsHash = 50,
    Semicolon = 51,
    Apostrophe = 52,
    Grave = 53,
    Comma = 54,
    Period = 55,
    Slash = 56,
    CapsLock = 57,
    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,
    PrintScreen = 70,
    ScrollLock = 71,
    Pause = 72,
    Insert = 73,
    Home = 74,
    PageUp = 75,
    Delete = 76,
    End = 77,
    PageDown = 78,
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82,
    NumLockClear = 83,
    NumpadDivide = 84,
    NumpadMultiply = 85,
    NumpadMinus = 86,
    NumpadPlus = 87,
    NumpadEnter = 88,
    Numpad1 = 89,
    Numpad2 = 90,
    Numpad3 = 91,
    Numpad4 = 92,
    Numpad5 = 93,
    Numpad6 = 94,
    Numpad7 = 95,
    Numpad8 = 96,
    Numpad9 = 97,
    Numpad0 = 98,
    NumpadPeriod = 99,
    NonUsBackslash = 100,
    Application = 101,
    Power = 102,
    NumpadEquals = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    Execute = 116,
    Help = 117,
    Menu = 118,
    Select = 119,
    Stop = 120,
    Again = 121,
    Undo = 122,
    Cut = 123,
    Copy = 124,
    Paste = 125,
    Find = 126,
    Mute = 127,
    VolumeUp = 128,
    VolumeDown = 129,
    NumpadComma = 133,
    NumpadEqualsAs400 = 134,
    International1 = 135,
    International2 = 136,
    International3 = 137,
    International4 = 138,
    International5 = 139,
    International6 = 140,
    International7 = 141,
    International8 = 142,
    International9 = 143,
    Lang1 = 144,
    Lang2 = 145,
    Lang3 = 146,
    Lang4 = 147,
    Lang5 = 148,
    Lang6 = 149,
    Lang7 = 150,
    Lang8 = 151,
    Lang9 = 152,
    AltErase = 153,
    SysReq = 154,
    Cancel = 155,
    Clear = 156,
    Prior = 157,
    Return2 = 158,
    Separator = 159,
    Out = 160,
    Oper = 161,
    ClearAgain = 162,
    CrSel = 163,
    ExSel = 164,
    Numpad00 = 176,
    Numpad000 = 177,
    ThousandsSeparator = 178,
    DecimalSeparator = 179,
    CurrencyUnit = 180,
    CurrencySubUnit = 181,
    NumpadLeftParen = 182,
    NumpadRightParen = 183,
    NumpadLeftBrace = 184,
    NumpadRightBrace = 185,
    NumpadTab = 186,
    NumpadBackspace = 187,
    NumpadA = 188,
    NumpadB = 189,
    NumpadC = 190,
    NumpadD = 191,
    NumpadE = 192,
    NumpadF = 193,
    NumpadXor = 194,
    NumpadPower = 195,
    NumpadPercent = 196,
    NumpadLess = 197,
    NumpadGreater = 198,
    NumpadAmpersand = 199,
    NumpadDblAmpersand = 200,
    NumpadVerticalBar = 201,
    NumpadDblVerticalBar = 202,
    NumpadColon = 203,
    NumpadHash = 204,
    NumpadSpace = 205,
    NumpadAt = 206,
    NumpadExclam = 207,
    NumpadMemStore = 208,
    NumpadMemRecall = 209,
    NumpadMemClear = 210,
    NumpadMemAdd = 211,
    NumpadMemSubtract = 212,
    NumpadMemMultiply = 213,
    NumpadMemDivide = 214,
    NumpadPlusMinus = 215,
    NumpadClear = 216,
    NumpadClearEntry = 217,
    NumpadBinary = 218,
    NumpadOctal = 219,
    NumpadDecimal = 220,
    NumpadHexadecimal = 221,
    LeftCtrl = 224,
    LeftShift = 225,
    LeftAlt = 226,
    LeftGui = 227,
    RightCtrl = 228,
    RightShift = 229,
    RightAlt = 230,
    RightGui = 231,
    Mode = 257,
  };

  namespace details {
    constexpr uint32_t to_keycode(Scancode scancode)
    {
      return static_cast<uint32_t>(scancode) | (1 << 30);
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
    QuoteDbl = '"',
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
    Backquote = '`',
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

  struct GF_GRAPHICS_API Keyboard {
    static const char* scancode_name(Scancode scancode);
    static Scancode scancode_from_name(const char* name);

    static const char* keycode_name(Keycode keycode);
    static Keycode keycode_from_name(const char* name);

    static Keycode localize(Scancode scancode);
    static Scancode unlocalize(Keycode keycode);
  };

} // namespace gf

#endif // GF_KEYBOARD_H
