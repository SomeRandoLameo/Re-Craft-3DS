#pragma once

#include "BlockFlower.hpp"

class BlockYellowFlower : public BlockFlower {
public:
    BlockYellowFlower() : BlockFlower() {}

    FlowerColor getBlockType() const override {
        return FlowerColor::Yellow;
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Yellow_Flower;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};