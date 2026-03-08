#pragma once

#include "Material.hpp"

class MaterialTransparent : public Material {
public:
    explicit MaterialTransparent(const MapColor& color);
    bool blocksLight() const override;
    bool isSolid() const override;
};
