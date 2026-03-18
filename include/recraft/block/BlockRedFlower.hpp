#pragma once

#include "BlockFlower.hpp"

class BlockRedFlower : public BlockFlower {
public:
    BlockRedFlower() : BlockFlower() {}

    FlowerColor getBlockType() const override {
        return FlowerColor::Red;
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Red_Flower;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return meta > 8 ? 0 : meta;
    }
};