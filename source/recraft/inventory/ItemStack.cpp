//
// Created by elias on 29.10.25.
//
#include "ItemStack.h"

void ItemStack_Transfer(ItemStack* src, ItemStack* dst) {
    if ((src->block == dst->block && src->meta == dst->meta) || dst->amount == 0) {
        int vol = MIN(src->amount, ITEMSTACK_MAX - dst->amount);
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