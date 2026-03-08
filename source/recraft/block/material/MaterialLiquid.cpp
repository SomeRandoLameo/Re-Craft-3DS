#include "block/material/MaterialLiquid.hpp"

MaterialLiquid::MaterialLiquid(const MapColor& color) : Material(color) {
    setReplaceable();
    setNoPushMobility();
}

bool MaterialLiquid::isLiquid() const { return true; }
bool MaterialLiquid::blocksMovement() const { return false; }
bool MaterialLiquid::isSolid() const { return false; }
