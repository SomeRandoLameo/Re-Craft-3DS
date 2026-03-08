#pragma once

#include "Material.hpp"

class MaterialPortal : public Material {
public:
    explicit MaterialPortal(const MapColor& color);
    bool isSolid() const override;
    bool blocksMovement() const override;
};
