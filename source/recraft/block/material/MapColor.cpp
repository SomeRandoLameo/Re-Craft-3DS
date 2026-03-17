#include "block/material/MapColor.hpp"

const MapColor MapColor::AIR(0, 0);
const MapColor MapColor::GRASS(1, 8368696);
const MapColor MapColor::SAND(2, 16247203);
const MapColor MapColor::CLOTH(3, 13092807);
const MapColor MapColor::TNT(4, 16711680);
const MapColor MapColor::ICE(5, 10526975);
const MapColor MapColor::IRON(6, 10987431);
const MapColor MapColor::FOLIAGE(7, 31744);
const MapColor MapColor::SNOW(8, 16777215);
const MapColor MapColor::CLAY(9, 10791096);
const MapColor MapColor::DIRT(10, 9923917);
const MapColor MapColor::STONE(11, 7368816);
const MapColor MapColor::WATER(12, 4210943);
const MapColor MapColor::WOOD(13, 9402184);
const MapColor MapColor::QUARTZ(14, 16776437);
const MapColor MapColor::ADOBE(15, 14188339);
const MapColor MapColor::MAGENTA(16, 11685080);
const MapColor MapColor::LIGHT_BLUE(17, 6724056);
const MapColor MapColor::YELLOW(18, 15066419);
const MapColor MapColor::LIME(19, 8375321);
const MapColor MapColor::PINK(20, 15892389);
const MapColor MapColor::GRAY(21, 5000268);
const MapColor MapColor::SILVER(22, 10066329);
const MapColor MapColor::CYAN(23, 5013401);
const MapColor MapColor::PURPLE(24, 8339378);
const MapColor MapColor::BLUE(25, 3361970);
const MapColor MapColor::BROWN(26, 6704179);
const MapColor MapColor::GREEN(27, 6717235);
const MapColor MapColor::RED(28, 10040115);
const MapColor MapColor::BLACK(29, 1644825);
const MapColor MapColor::GOLD(30, 16445005);
const MapColor MapColor::DIAMOND(31, 6085589);
const MapColor MapColor::LAPIS(32, 4882687);
const MapColor MapColor::EMERALD(33, 55610);
const MapColor MapColor::OBSIDIAN(34, 8476209);
const MapColor MapColor::NETHERRACK(35, 7340544);
// TODO: What are these?
const MapColor MapColor::field_193561_M(36, 13742497);
const MapColor MapColor::field_193562_N(37, 10441252);
const MapColor MapColor::field_193563_O(38, 9787244);
const MapColor MapColor::field_193564_P(39, 7367818);
const MapColor MapColor::field_193565_Q(40, 12223780);
const MapColor MapColor::field_193566_R(41, 6780213);
const MapColor MapColor::field_193567_S(42, 10505550);
const MapColor MapColor::field_193568_T(43, 3746083);
const MapColor MapColor::field_193569_U(44, 8874850);
const MapColor MapColor::field_193570_V(45, 5725276);
const MapColor MapColor::field_193571_W(46, 8014168);
const MapColor MapColor::field_193572_X(47, 4996700);
const MapColor MapColor::field_193573_Y(48, 4993571);
const MapColor MapColor::field_193574_Z(49, 5001770);
const MapColor MapColor::field_193559_aa(50, 9321518);
const MapColor MapColor::field_193560_ab(51, 2430480);

const MapColor* MapColor::COLORS[64] = {};
const MapColor* MapColor::field_193575_b[16] = {}; // TODO: What are these?
bool MapColor::s_dyeMappingInitialized = false;

MapColor::MapColor(int index, int color) : colorIndex(index), colorValue(color) {
    if (index < 0 || index > 63)
        throw std::out_of_range("Map color ID must be between 0 and 63 (inclusive)");

    COLORS[index] = this;
}

void MapColor::initDyeColorMapping() {
    if (s_dyeMappingInitialized)
        return;

    field_193575_b[EnumDyeColor::WHITE().getMetadata()] = &SNOW;
    field_193575_b[EnumDyeColor::ORANGE().getMetadata()] = &ADOBE;
    field_193575_b[EnumDyeColor::MAGENTA().getMetadata()] = &MAGENTA;
    field_193575_b[EnumDyeColor::LIGHT_BLUE().getMetadata()] = &LIGHT_BLUE;
    field_193575_b[EnumDyeColor::YELLOW().getMetadata()] = &YELLOW;
    field_193575_b[EnumDyeColor::LIME().getMetadata()] = &LIME;
    field_193575_b[EnumDyeColor::PINK().getMetadata()] = &PINK;
    field_193575_b[EnumDyeColor::GRAY().getMetadata()] = &GRAY;
    field_193575_b[EnumDyeColor::SILVER().getMetadata()] = &SILVER;
    field_193575_b[EnumDyeColor::CYAN().getMetadata()] = &CYAN;
    field_193575_b[EnumDyeColor::PURPLE().getMetadata()] = &PURPLE;
    field_193575_b[EnumDyeColor::BLUE().getMetadata()] = &BLUE;
    field_193575_b[EnumDyeColor::BROWN().getMetadata()] = &BROWN;
    field_193575_b[EnumDyeColor::GREEN().getMetadata()] = &GREEN;
    field_193575_b[EnumDyeColor::RED().getMetadata()] = &RED;
    field_193575_b[EnumDyeColor::BLACK().getMetadata()] = &BLACK;

    s_dyeMappingInitialized = true;
}

int MapColor::getMapColor(int shade) const {
    int brightness = 220;

    if (shade == 3)
        brightness = 135;
    if (shade == 2)
        brightness = 255;
    if (shade == 1)
        brightness = 220;
    if (shade == 0)
        brightness = 180;

    int r = ((colorValue >> 16) & 0xFF) * brightness / 255;
    int g = ((colorValue >> 8) & 0xFF) * brightness / 255;
    int b = ((colorValue) & 0xFF) * brightness / 255;

    return static_cast<int>(0xFF000000) | (r << 16) | (g << 8) | b;
}

const MapColor& MapColor::fromDyeColor(const EnumDyeColor& dyeColor) {
    initDyeColorMapping();
    return *field_193575_b[dyeColor.getMetadata()];
}

bool MapColor::operator==(const MapColor& other) const { return colorIndex == other.colorIndex; }
bool MapColor::operator!=(const MapColor& other) const { return !(*this == other); }
