//
// Created by elias on 29.10.25.
//
#include "ItemStack.h"

void ItemStack::Transfer(ItemStack* src, ItemStack* dst) {
    if ((src->block == dst->block && src->meta == dst->meta) || dst->amount == 0) {
        int vol = std::min(static_cast<int>(src->amount), MAX - static_cast<int>(dst->amount));
        src->amount -= vol;
        dst->amount += vol;
        dst->block = src->block;
        dst->meta = src->meta;
    } else {
        ItemStack tmp = *src;
        *src = *dst;
        *dst = tmp;
    }
}
