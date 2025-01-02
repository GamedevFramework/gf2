// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BIN_PACK_H
#define GF_BIN_PACK_H

#include <cstdint>

#include <optional>
#include <tuple>
#include <vector>

#include "CoreApi.h"
#include "Rect.h"
#include "Vec2.h"

namespace gf {

  enum class BinPackChoice : uint8_t {
    BestArea,
    BestShortSide,
    BestLongSide,
    WorstArea,
    WorstShortSide,
    WorstLongSide,
  };

  enum class BinPackSplit : uint8_t {
    ShorterLeftoverAxis,
    LongerLeftoverAxis,
    MinimumArea,
    MaximumArea,
    ShorterAxis,
    LongerAxis,
  };

  enum class BinPackSort : uint8_t {
    AreaAsc,
    AreaDesc,
    ShortSideAsc,
    ShortSideDesc,
    LongSideAsc,
    LongSideDesc,
    PerimeterAsc,
    PermiterDesc,
    SideDifferenceAsc,
    SideDifferenceDesc,
    SideRatioAsc,
    SideRatioDesc,
  };

  class GF_CORE_API BinPack {
  public:
    BinPack(Vec2I size);

    std::optional<RectI> insert(Vec2I size, BinPackChoice choice = BinPackChoice::BestArea, BinPackSplit split = BinPackSplit::MinimumArea);

    std::optional<std::vector<RectI>> insert(const std::vector<Vec2I>& sizes, BinPackChoice choice = BinPackChoice::BestShortSide, BinPackSplit split = BinPackSplit::ShorterAxis, BinPackSort sort = BinPackSort::ShortSideDesc);

  private:
    std::tuple<std::size_t, RectI> find_rectangle(Vec2I size, BinPackChoice choice) const;
    void split_rectangle(std::size_t index, RectI rectangle, BinPackSplit split);
    void rectangle_merge();

    void remove_rectangle(std::size_t index);

    std::vector<RectI> m_free_rectangles;
  };

}

#endif // GF_BIN_PACK_H
