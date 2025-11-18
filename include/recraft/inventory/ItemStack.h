#pragma once

#include <cstdint>
#include <algorithm>

#include "../../mclib/common/Common.h"
#include "../../mclib/core/Client.h"
#include "../../mclib/util/Forge.h"
#include "../../mclib/util/Hash.h"
#include "../../mclib/util/Utility.h"
#include "../../mclib/util/VersionFetcher.h"

#include "../blocks/CT_Block.h"

#include "../misc/NumberUtils.h"

class ItemStack {
public:
    Block block;
    u8 meta; // ItemDamage
    u8 amount;

    static constexpr int MAX = 64;
};
