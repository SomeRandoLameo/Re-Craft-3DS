#include "block/material/MaterialLogic.hpp"

MaterialLogic::MaterialLogic(const MapColor& color) : Material(color) {}

bool MaterialLogic::isSolid() const { return false; }
bool MaterialLogic::blocksLight() const { return false; }
