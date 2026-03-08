#pragma once

#include "Material.hpp"

class MaterialWeb : public Material {
public:
    explicit MaterialWeb(const MapColor& color);
    bool blocksMovement() const override;
};
