#pragma once

#include "../mclib.h"
#include "Vector.h"

namespace mc {

    enum DyeColor {
        Black,
        Red,
        Green,
        Brown,
        Blue,
        Purple,
        Cyan,
        LightGray,
        Gray,
        Pink,
        Lime,
        Yellow,
        LightBlue,
        Magenta,
        Orange,
        White,
    };

    Vector3d GetColor(DyeColor dyeColor);
    std::string to_string(DyeColor dyeColor);
    std::wstring to_wstring(DyeColor dyeColor);

} // ns mc