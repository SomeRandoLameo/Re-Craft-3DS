#include "block/material/MaterialPortal.hpp"

MaterialPortal::MaterialPortal(const MapColor& color) : Material(color) {}

bool MaterialPortal::isSolid() const { return false; }
bool MaterialPortal::blocksMovement() const { return false; }
