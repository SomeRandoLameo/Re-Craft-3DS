#pragma once

#include <stdbool.h>

#include "../../mclib/common/Common.h"
#include "../../mclib/core/Client.h"
#include "../../mclib/util/Forge.h"
#include "../../mclib/util/Hash.h"
#include "../../mclib/util/Utility.h"
#include "../../mclib/util/VersionFetcher.h"

#include "../blocks/CT_Block.h"

#include "../misc/NumberUtils.h"

typedef struct {
	Block block;
	uint8_t meta, amount;
} ItemStack;

#define ITEMSTACK_MAX (64)

void ItemStack_Transfer(ItemStack* src, ItemStack* dst);