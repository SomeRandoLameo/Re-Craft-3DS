#include "item/EnumDyeColor.hpp"

#include <stdexcept>

const EnumDyeColor EnumDyeColor::WHITE(0, 15, "white", "white", 16383998, TextFormatting::WHITE);
const EnumDyeColor EnumDyeColor::ORANGE(1, 14, "orange", "orange", 16351261, TextFormatting::GOLD);
const EnumDyeColor EnumDyeColor::MAGENTA(2, 13, "magenta", "magenta", 13061821, TextFormatting::AQUA);
const EnumDyeColor EnumDyeColor::LIGHT_BLUE(3, 12, "light_blue", "lightBlue", 3847130, TextFormatting::BLUE);
const EnumDyeColor EnumDyeColor::YELLOW(4, 11, "yellow", "yellow", 16701501, TextFormatting::YELLOW);
const EnumDyeColor EnumDyeColor::LIME(5, 10, "lime", "lime", 8439583, TextFormatting::GREEN);
const EnumDyeColor EnumDyeColor::PINK(6, 9, "pink", "pink", 15961002, TextFormatting::LIGHT_PURPLE);
const EnumDyeColor EnumDyeColor::GRAY(7, 8, "gray", "gray", 4673362, TextFormatting::DARK_GRAY);
const EnumDyeColor EnumDyeColor::SILVER(8, 7, "silver", "silver", 10329495, TextFormatting::GRAY);
const EnumDyeColor EnumDyeColor::CYAN(9, 6, "cyan", "cyan", 1481884, TextFormatting::DARK_AQUA);
const EnumDyeColor EnumDyeColor::PURPLE(10, 5, "purple", "purple", 8991416, TextFormatting::DARK_PURPLE);
const EnumDyeColor EnumDyeColor::BLUE(11, 4, "blue", "blue", 3949738, TextFormatting::DARK_BLUE);
const EnumDyeColor EnumDyeColor::BROWN(12, 3, "brown", "brown", 8606770, TextFormatting::GOLD);
const EnumDyeColor EnumDyeColor::GREEN(13, 2, "green", "green", 6192150, TextFormatting::DARK_GREEN);
const EnumDyeColor EnumDyeColor::RED(14, 1, "red", "red", 11546150, TextFormatting::DARK_RED);
const EnumDyeColor EnumDyeColor::BLACK(15, 0, "black", "black", 1908001, TextFormatting::BLACK);

const EnumDyeColor* EnumDyeColor::s_metaLookup[16] = {};
const EnumDyeColor* EnumDyeColor::s_dyeDamageLookup[16] = {};
bool EnumDyeColor::s_lookupsInitialized = false;

EnumDyeColor::EnumDyeColor(int meta, int dyeDamage, const char* name, const char* unlocalizedName, int colorValue,
                           TextFormatting chatColor) :
    m_meta(meta), m_dyeDamage(dyeDamage), m_name(name), m_unlocalizedName(unlocalizedName), m_colorValue(colorValue),
    m_chatColor(chatColor) {
    int r = (colorValue & 0xFF0000) >> 16;
    int g = (colorValue & 0x00FF00) >> 8;
    int b = (colorValue & 0x0000FF);
    m_colorComponents = {r / 255.0f, g / 255.0f, b / 255.0f};
}

void EnumDyeColor::initLookups() {
    if (s_lookupsInitialized)
        return;

    const EnumDyeColor* all[] = {&WHITE,  &ORANGE, &MAGENTA, &LIGHT_BLUE, &YELLOW, &LIME,  &PINK, &GRAY,
                                 &SILVER, &CYAN,   &PURPLE,  &BLUE,       &BROWN,  &GREEN, &RED,  &BLACK};

    for (const EnumDyeColor* color : all) {
        s_metaLookup[color->m_meta] = color;
        s_dyeDamageLookup[color->m_dyeDamage] = color;
    }

    s_lookupsInitialized = true;
}

const EnumDyeColor& EnumDyeColor::byMetadata(int meta) {
    initLookups();
    if (meta < 0 || meta >= 16)
        meta = 0;
    return *s_metaLookup[meta];
}

const EnumDyeColor& EnumDyeColor::byDyeDamage(int damage) {
    initLookups();
    if (damage < 0 || damage >= 16)
        damage = 0;
    return *s_dyeDamageLookup[damage];
}


int EnumDyeColor::getMetadata() const { return m_meta; }
int EnumDyeColor::getDyeDamage() const { return m_dyeDamage; }
int EnumDyeColor::getColorValue() const { return m_colorValue; }
const std::string& EnumDyeColor::getName() const { return m_name; }
const std::string& EnumDyeColor::getUnlocalizedName() const { return m_unlocalizedName; }
const std::array<float, 3>& EnumDyeColor::getColorComponentValues() const { return m_colorComponents; }
TextFormatting EnumDyeColor::getChatColor() const { return m_chatColor; }

// IStringSerializable
const std::string& EnumDyeColor::getSerializedName() const { return m_name; }

std::string EnumDyeColor::toString() const { return m_unlocalizedName; }

bool EnumDyeColor::operator==(const EnumDyeColor& other) const { return m_meta == other.m_meta; }
bool EnumDyeColor::operator!=(const EnumDyeColor& other) const { return !(*this == other); }
