#include "block/material/MaterialTransparent.hpp"

MaterialTransparent::MaterialTransparent(const MapColor& color) : Material(color) {}

bool MaterialTransparent::blocksLight() const { return false; }
bool MaterialTransparent::isSolid() const { return false; }
