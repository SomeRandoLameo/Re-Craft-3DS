#include "block/material/MaterialWeb.hpp"

MaterialWeb::MaterialWeb(const MapColor& color) : Material(color) {}
bool MaterialWeb::blocksMovement() const { return false; }
