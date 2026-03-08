#pragma once

#include "Material.hpp"

class MaterialLogic : public Material {
public:
    explicit MaterialLogic(const MapColor& color);
    bool isSolid() const override;
    bool blocksLight() const override;
};
