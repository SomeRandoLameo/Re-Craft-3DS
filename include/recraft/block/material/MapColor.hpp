#pragma once

#include <stdexcept>

#include "item/EnumDyeColor.hpp"

class MapColor {
public:
    static const MapColor AIR;
    static const MapColor GRASS;
    static const MapColor SAND;
    static const MapColor CLOTH;
    static const MapColor TNT;
    static const MapColor ICE;
    static const MapColor IRON;
    static const MapColor FOLIAGE;
    static const MapColor SNOW;
    static const MapColor CLAY;
    static const MapColor DIRT;
    static const MapColor STONE;
    static const MapColor WATER;
    static const MapColor WOOD;
    static const MapColor QUARTZ;
    static const MapColor ADOBE;
    static const MapColor MAGENTA;
    static const MapColor LIGHT_BLUE;
    static const MapColor YELLOW;
    static const MapColor LIME;
    static const MapColor PINK;
    static const MapColor GRAY;
    static const MapColor SILVER;
    static const MapColor CYAN;
    static const MapColor PURPLE;
    static const MapColor BLUE;
    static const MapColor BROWN;
    static const MapColor GREEN;
    static const MapColor RED;
    static const MapColor BLACK;
    static const MapColor GOLD;
    static const MapColor DIAMOND;
    static const MapColor LAPIS;
    static const MapColor EMERALD;
    static const MapColor OBSIDIAN;
    static const MapColor NETHERRACK;

    // indices 36-51
    static const MapColor field_193561_M;
    static const MapColor field_193562_N;
    static const MapColor field_193563_O;
    static const MapColor field_193564_P;
    static const MapColor field_193565_Q;
    static const MapColor field_193566_R;
    static const MapColor field_193567_S;
    static const MapColor field_193568_T;
    static const MapColor field_193569_U;
    static const MapColor field_193570_V;
    static const MapColor field_193571_W;
    static const MapColor field_193572_X;
    static const MapColor field_193573_Y;
    static const MapColor field_193574_Z;
    static const MapColor field_193559_aa;
    static const MapColor field_193560_ab;

    static const MapColor* COLORS[64];
    static const MapColor* field_193575_b[16];

    static const MapColor& fromDyeColor(const EnumDyeColor& dyeColor);

    const int colorIndex;
    const int colorValue;

    int getMapColor(int shade) const;

    bool operator==(const MapColor& other) const;
    bool operator!=(const MapColor& other) const;

private:
    MapColor(int index, int color);

    static void initDyeColorMapping();
    static bool s_dyeMappingInitialized;
};
