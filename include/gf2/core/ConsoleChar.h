// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_CHAR_H
#define GF_CONSOLE_CHAR_H

#include "CoreApi.h"

namespace gf {

  struct GF_CORE_API ConsoleChar final {
    ConsoleChar() = delete;

    static constexpr char16_t WhiteSmilingFace = u'☺';
    static constexpr char16_t BlackSmilingFace = u'☻';
    static constexpr char16_t BlackHeartSuit = u'♥';
    static constexpr char16_t BlackDiamondSuit = u'♦';
    static constexpr char16_t BlackClubSuit = u'♣';
    static constexpr char16_t BlackSpadeSuit = u'♠';
    static constexpr char16_t Bullet = u'•';
    static constexpr char16_t InverseBullet = u'◘';
    static constexpr char16_t WhiteCircle = u'○';
    static constexpr char16_t InverseWhiteCircle = u'◙';
    static constexpr char16_t MaleSign = u'♂';
    static constexpr char16_t FemaleSign = u'♀';
    static constexpr char16_t EighthNote = u'♪';
    static constexpr char16_t BeamedEighthNotes = u'♫';
    static constexpr char16_t WhiteSunWithRays = u'☼';
    static constexpr char16_t BlackRightPointingPointer = u'►';
    static constexpr char16_t BlackLeftPointingPointer = u'◄';
    static constexpr char16_t UpDownArrow = u'↕';
    static constexpr char16_t DoubleExclamationMark = u'‼';
    static constexpr char16_t PilcrowSign = u'¶';
    static constexpr char16_t SectionSign = u'§';
    static constexpr char16_t BlackRectangle = u'▬';
    static constexpr char16_t UpDownArrowWithBase = u'↨';
    static constexpr char16_t UpwardsArrow = u'↑';
    static constexpr char16_t DownwardsArrow = u'↓';
    static constexpr char16_t RightwardsArrow = u'→';
    static constexpr char16_t LeftwardsArrow = u'←';
    static constexpr char16_t RightAngle = u'∟';
    static constexpr char16_t LeftRightArrow = u'↔';
    static constexpr char16_t BlackUpPointingTriangle = u'▲';
    static constexpr char16_t BlackDownPointingTriangle = u'▼';
    static constexpr char16_t House = u'⌂';
    static constexpr char16_t CentSign = u'¢';
    static constexpr char16_t PoundSign = u'£';
    static constexpr char16_t YenSign = u'¥';
    static constexpr char16_t PesetaSign = u'₧';
    static constexpr char16_t LatinSmallLetterFWithHook = u'ƒ';
    static constexpr char16_t FeminineOrdinalIndicator = u'ª';
    static constexpr char16_t MasculineOrdinalIndicator = u'º';
    static constexpr char16_t InvertedQuestionMark = u'¿';
    static constexpr char16_t ReversedNotSign = u'⌐';
    static constexpr char16_t NotSign = u'¬';
    static constexpr char16_t VulgarFractionOneHalf = u'½';
    static constexpr char16_t VulgarFractionOneQuarter = u'¼';
    static constexpr char16_t InvertedExclamationMark = u'¡';
    static constexpr char16_t LeftPointingDoubleAngleQuotationMark = u'«';
    static constexpr char16_t RightPointingDoubleAngleQuotationMark = u'»';
    static constexpr char16_t LightShade = u'░';
    static constexpr char16_t MediumShade = u'▒';
    static constexpr char16_t DarkShade = u'▓';
    static constexpr char16_t BoxDrawingsLightVertical = u'│';
    static constexpr char16_t BoxDrawingsLightVerticalAndLeft = u'┤';
    static constexpr char16_t BoxDrawingsVerticalSingleAndLeftDouble = u'╡';
    static constexpr char16_t BoxDrawingsVerticalDoubleAndLeftSingle = u'╢';
    static constexpr char16_t BoxDrawingsDownDoubleAndLeftSingle = u'╖';
    static constexpr char16_t BoxDrawingsDownSingleAndLeftDouble = u'╕';
    static constexpr char16_t BoxDrawingsDoubleVerticalAndLeft = u'╣';
    static constexpr char16_t BoxDrawingsDoubleVertical = u'║';
    static constexpr char16_t BoxDrawingsDoubleDownAndLeft = u'╗';
    static constexpr char16_t BoxDrawingsDoubleUpAndLeft = u'╝';
    static constexpr char16_t BoxDrawingsUpDoubleAndLeftSingle = u'╜';
    static constexpr char16_t BoxDrawingsUpSingleAndLeftDouble = u'╛';
    static constexpr char16_t BoxDrawingsLightDownAndLeft = u'┐';
    static constexpr char16_t BoxDrawingsLightUpAndRight = u'└';
    static constexpr char16_t BoxDrawingsLightUpAndHorizontal = u'┴';
    static constexpr char16_t BoxDrawingsLightDownAndHorizontal = u'┬';
    static constexpr char16_t BoxDrawingsLightVerticalAndRight = u'├';
    static constexpr char16_t BoxDrawingsLightHorizontal = u'─';
    static constexpr char16_t BoxDrawingsLightVerticalAndHorizontal = u'┼';
    static constexpr char16_t BoxDrawingsVerticalSingleAndRightDouble = u'╞';
    static constexpr char16_t BoxDrawingsVerticalDoubleAndRightSingle = u'╟';
    static constexpr char16_t BoxDrawingsDoubleUpAndRight = u'╚';
    static constexpr char16_t BoxDrawingsDoubleDownAndRight = u'╔';
    static constexpr char16_t BoxDrawingsDoubleUpAndHorizontal = u'╩';
    static constexpr char16_t BoxDrawingsDoubleDownAndHorizontal = u'╦';
    static constexpr char16_t BoxDrawingsDoubleVerticalAndRight = u'╠';
    static constexpr char16_t BoxDrawingsDoubleHorizontal = u'═';
    static constexpr char16_t BoxDrawingsDoubleVerticalAndHorizontal = u'╬';
    static constexpr char16_t BoxDrawingsUpSingleAndHorizontalDouble = u'╧';
    static constexpr char16_t BoxDrawingsUpDoubleAndHorizontalSingle = u'╨';
    static constexpr char16_t BoxDrawingsDownSingleAndHorizontalDouble = u'╤';
    static constexpr char16_t BoxDrawingsDownDoubleAndHorizontalSingle = u'╥';
    static constexpr char16_t BoxDrawingsUpDoubleAndRightSingle = u'╙';
    static constexpr char16_t BoxDrawingsUpSingleAndRightDouble = u'╘';
    static constexpr char16_t BoxDrawingsDownSingleAndRightDouble = u'╒';
    static constexpr char16_t BoxDrawingsDownDoubleAndRightSingle = u'╓';
    static constexpr char16_t BoxDrawingsVerticalDoubleAndHorizontalSingle = u'╫';
    static constexpr char16_t BoxDrawingsVerticalSingleAndHorizontalDouble = u'╪';
    static constexpr char16_t BoxDrawingsLightUpAndLeft = u'┘';
    static constexpr char16_t BoxDrawingsLightDownAndRight = u'┌';
    static constexpr char16_t FullBlock = u'█';
    static constexpr char16_t LowerHalfBlock = u'▄';
    static constexpr char16_t LeftHalfBlock = u'▌';
    static constexpr char16_t RightHalfBlock = u'▐';
    static constexpr char16_t UpperHalfBlock = u'▀';
    static constexpr char16_t GreekSmallLetterAlpha = u'α';
    static constexpr char16_t LatinSmallLetterSharpS = u'ß';
    static constexpr char16_t GreekCapitalLetterGamma = u'Γ';
    static constexpr char16_t GreekSmallLetterPi = u'π';
    static constexpr char16_t GreekCapitalLetterSigma = u'Σ';
    static constexpr char16_t GreekSmallLetterSigma = u'σ';
    static constexpr char16_t MicroSign = u'µ';
    static constexpr char16_t GreekSmallLetterTau = u'τ';
    static constexpr char16_t GreekCapitalLetterPhi = u'Φ';
    static constexpr char16_t GreekCapitalLetterTheta = u'Θ';
    static constexpr char16_t GreekCapitalLetterOmega = u'Ω';
    static constexpr char16_t GreekSmallLetterDelta = u'δ';
    static constexpr char16_t Infinity = u'∞';
    static constexpr char16_t GreekSmallLetterPhi = u'φ';
    static constexpr char16_t GreekSmallLetterEpsilon = u'ε';
    static constexpr char16_t Intersection = u'∩';
    static constexpr char16_t IdenticalTo = u'≡';
    static constexpr char16_t PlusMinusSign = u'±';
    static constexpr char16_t GreaterThanOrEqualTo = u'≥';
    static constexpr char16_t LessThanOrEqualTo = u'≤';
    static constexpr char16_t TopHalfIntegral = u'⌠';
    static constexpr char16_t BottomHalfIntegral = u'⌡';
    static constexpr char16_t DivisionSign = u'÷';
    static constexpr char16_t AlmostEqualTo = u'≈';
    static constexpr char16_t DegreeSign = u'°';
    static constexpr char16_t BulletOperator = u'∙';
    static constexpr char16_t MiddleDot = u'·';
    static constexpr char16_t SquareRoot = u'√';
    static constexpr char16_t SuperscriptLatinSmallLetterN = u'ⁿ';
    static constexpr char16_t SuperscriptTwo = u'²';
    static constexpr char16_t BlackSquare = u'■';
    static constexpr char16_t NoBreakSpace = u'\u00A0';
    static constexpr char16_t GreekSmallLetterBeta = u'β';
    static constexpr char16_t GreekCapitalLetterPi = u'Π';
    static constexpr char16_t NAryProduct = u'∏';
    static constexpr char16_t NArySummation = u'∑';
    static constexpr char16_t GreekSmallLetterMu = u'μ';
    static constexpr char16_t OhmSign = u'Ω';
    static constexpr char16_t GreekPhiSymbol = u'ϕ';
    static constexpr char16_t EmptySet = u'∅';
    static constexpr char16_t DiameterSign = u'⌀';
    static constexpr char16_t LatinCapitalLetterOWithStroke = u'Ø';
    static constexpr char16_t LatinSmallLetterOWithStroke = u'ø';
    static constexpr char16_t ElementOf = u'∈';
    static constexpr char16_t QuadrantUpperLeft = u'▘';
    static constexpr char16_t QuadrantUpperRight = u'▝';
    static constexpr char16_t QuadrantLowerRight = u'▗';
    static constexpr char16_t QuadrantUpperLeftAndLowerRight = u'▚';
    static constexpr char16_t QuadrantLowerLeft = u'▖';
  };

}

#endif // GF_CONSOLE_CHAR_H
