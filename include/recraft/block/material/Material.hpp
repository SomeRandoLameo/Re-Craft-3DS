#pragma once

#include "EnumPushReaction.hpp"
#include "MapColor.hpp"
class Material {
public:
    // Static material instances
    static Material* AIR;
    static Material* GRASS;
    static Material* GROUND;
    static Material* WOOD;
    static Material* ROCK;
    static Material* IRON;
    static Material* ANVIL;
    static Material* WATER;
    static Material* LAVA;
    static Material* LEAVES;
    static Material* PLANTS;
    static Material* VINE;
    static Material* SPONGE;
    static Material* CLOTH;
    static Material* FIRE;
    static Material* SAND;
    static Material* CIRCUITS;
    static Material* CARPET;
    static Material* GLASS;
    static Material* REDSTONE_LIGHT;
    static Material* TNT;
    static Material* CORAL;
    static Material* ICE;
    static Material* PACKED_ICE;
    static Material* SNOW;
    static Material* CRAFTED_SNOW;
    static Material* CACTUS;
    static Material* CLAY;
    static Material* GOURD;
    static Material* DRAGON_EGG;
    static Material* PORTAL;
    static Material* CAKE;
    static Material* WEB;
    static Material* PISTON;
    static Material* BARRIER;
    static Material* STRUCTURE_VOID;

    explicit Material(const MapColor& color);
    virtual ~Material() = default;

    virtual bool isLiquid() const;
    virtual bool isSolid() const;
    virtual bool blocksLight() const;
    virtual bool blocksMovement() const;

    bool getCanBurn() const;
    bool isReplaceable() const;
    bool isOpaque() const;
    bool isToolNotRequired() const;

    EnumPushReaction getMobilityFlag() const;
    const MapColor& getMaterialMapColor() const;

    Material* setReplaceable();

protected:
    Material* setRequiresTool();
    Material* setBurning();
    Material* setNoPushMobility();
    Material* setImmovableMobility();
    Material* setAdventureModeExempt();

private:
    Material* setTranslucent();

    bool canBurn = false;
    bool replaceable = false;
    bool isTranslucent = false;
    bool requiresNoTool = true;
    bool isAdventureModeExemptFlag = false;
    EnumPushReaction mobilityFlag = EnumPushReaction::NORMAL;
    const MapColor& materialMapColor;
};
