#include "block/material/Material.hpp"
#include "block/material/MaterialLiquid.hpp"
#include "block/material/MaterialLogic.hpp"
#include "block/material/MaterialPortal.hpp"
#include "block/material/MaterialTransparent.hpp"

// clang-format off
Material* Material::AIR = new MaterialTransparent(MapColor::AIR);
Material* Material::GRASS = new Material(MapColor::GRASS);
Material* Material::GROUND = new Material(MapColor::DIRT);
Material* Material::WOOD = (new Material(MapColor::WOOD))->setBurning();
Material* Material::ROCK = (new Material(MapColor::STONE))->setRequiresTool();
Material* Material::IRON = (new Material(MapColor::IRON))->setRequiresTool();
Material* Material::ANVIL = (new Material(MapColor::IRON))->setRequiresTool()->setImmovableMobility();
Material* Material::WATER = (new MaterialLiquid(MapColor::WATER))->setNoPushMobility();
Material* Material::LAVA = (new MaterialLiquid(MapColor::TNT))->setNoPushMobility();
Material* Material::LEAVES = (new Material(MapColor::FOLIAGE))->setBurning()->setTranslucent()->setNoPushMobility();
Material* Material::PLANTS = (new MaterialLogic(MapColor::FOLIAGE))->setNoPushMobility();
Material* Material::VINE = (new MaterialLogic(MapColor::FOLIAGE))->setBurning()->setNoPushMobility()->setReplaceable();
Material* Material::SPONGE = new Material(MapColor::YELLOW);
Material* Material::CLOTH = (new Material(MapColor::CLOTH))->setBurning();
Material* Material::FIRE = (new MaterialTransparent(MapColor::AIR))->setNoPushMobility();
Material* Material::SAND = new Material(MapColor::SAND);
Material* Material::CIRCUITS = (new MaterialLogic(MapColor::AIR))->setNoPushMobility();
Material* Material::CARPET = (new MaterialLogic(MapColor::CLOTH))->setBurning();
Material* Material::GLASS = (new Material(MapColor::AIR))->setTranslucent()->setAdventureModeExempt();
Material* Material::REDSTONE_LIGHT = (new Material(MapColor::AIR))->setAdventureModeExempt();
Material* Material::TNT = (new Material(MapColor::TNT))->setBurning()->setTranslucent();
Material* Material::CORAL = (new Material(MapColor::FOLIAGE))->setNoPushMobility();
Material* Material::ICE = (new Material(MapColor::ICE))->setTranslucent()->setAdventureModeExempt();
Material* Material::PACKED_ICE = (new Material(MapColor::ICE))->setAdventureModeExempt();
Material* Material::SNOW = (new MaterialLogic(MapColor::SNOW))->setReplaceable()->setTranslucent()->setRequiresTool()->setNoPushMobility();
Material* Material::CRAFTED_SNOW = (new Material(MapColor::SNOW))->setRequiresTool();
Material* Material::CACTUS = (new Material(MapColor::FOLIAGE))->setTranslucent()->setNoPushMobility();
Material* Material::CLAY = new Material(MapColor::CLAY);
Material* Material::GOURD = (new Material(MapColor::FOLIAGE))->setNoPushMobility();
Material* Material::DRAGON_EGG = (new Material(MapColor::FOLIAGE))->setNoPushMobility();
Material* Material::PORTAL = (new MaterialPortal(MapColor::AIR))->setImmovableMobility();
Material* Material::CAKE = (new Material(MapColor::AIR))->setNoPushMobility();
Material* Material::WEB = (new Material(MapColor::CLOTH))->setRequiresTool()->setNoPushMobility();
Material* Material::PISTON = (new Material(MapColor::STONE))->setImmovableMobility();
Material* Material::BARRIER = (new Material(MapColor::AIR))->setRequiresTool()->setImmovableMobility();
Material* Material::STRUCTURE_VOID = new MaterialTransparent(MapColor::AIR);
// clang-format on
Material::Material(const MapColor& color) : materialMapColor(color) {}

bool Material::isLiquid() const { return false; }
bool Material::isSolid() const { return true; }
bool Material::blocksLight() const { return true; }
bool Material::blocksMovement() const { return true; }

bool Material::getCanBurn() const { return canBurn; }
bool Material::isReplaceable() const { return replaceable; }
bool Material::isToolNotRequired() const { return requiresNoTool; }

bool Material::isOpaque() const { return isTranslucent ? false : blocksMovement(); }

EnumPushReaction Material::getMobilityFlag() const { return mobilityFlag; }
const MapColor& Material::getMaterialMapColor() const { return materialMapColor; }

Material* Material::setTranslucent() {
    isTranslucent = true;
    return this;
}

Material* Material::setRequiresTool() {
    requiresNoTool = false;
    return this;
}

Material* Material::setBurning() {
    canBurn = true;
    return this;
}

Material* Material::setReplaceable() {
    replaceable = true;
    return this;
}

Material* Material::setNoPushMobility() {
    mobilityFlag = EnumPushReaction::DESTROY;
    return this;
}

Material* Material::setImmovableMobility() {
    mobilityFlag = EnumPushReaction::BLOCK;
    return this;
}

Material* Material::setAdventureModeExempt() {
    isAdventureModeExemptFlag = true;
    return this;
}
