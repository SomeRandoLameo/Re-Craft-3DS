#pragma once

#include "Material.hpp"

class MaterialLiquid : public Material {
public:
    explicit MaterialLiquid(const MapColor& color);
    bool isLiquid() const override;
    bool isSolid() const override;
    bool blocksMovement() const override;
};
