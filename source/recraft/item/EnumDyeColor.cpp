#include "item/EnumDyeColor.hpp"

#include <stdexcept>

// ---------------------------------------------------------------------------
// Construct-on-first-use: each instance is a function-local static, so it is
// guaranteed to be initialised before any caller, and — crucially — after the
// TextFormatting instances it references (which use the same idiom).
// ---------------------------------------------------------------------------

const EnumDyeColor& EnumDyeColor::WHITE() {
    static const EnumDyeColor instance(0, 15, "white", "white", 16383998, TextFormatting::WHITE());
    return instance;
}
const EnumDyeColor& EnumDyeColor::ORANGE() {
    static const EnumDyeColor instance(1, 14, "orange", "orange", 16351261, TextFormatting::GOLD());
    return instance;
}
const EnumDyeColor& EnumDyeColor::MAGENTA() {
    static const EnumDyeColor instance(2, 13, "magenta", "magenta", 13061821, TextFormatting::AQUA());
    return instance;
}
const EnumDyeColor& EnumDyeColor::LIGHT_BLUE() {
    static const EnumDyeColor instance(3, 12, "light_blue", "lightBlue", 3847130, TextFormatting::BLUE());
    return instance;
}
const EnumDyeColor& EnumDyeColor::YELLOW() {
    static const EnumDyeColor instance(4, 11, "yellow", "yellow", 16701501, TextFormatting::YELLOW());
    return instance;
}
const EnumDyeColor& EnumDyeColor::LIME() {
    static const EnumDyeColor instance(5, 10, "lime", "lime", 8439583, TextFormatting::GREEN());
    return instance;
}
const EnumDyeColor& EnumDyeColor::PINK() {
    static const EnumDyeColor instance(6, 9, "pink", "pink", 15961002, TextFormatting::LIGHT_PURPLE());
    return instance;
}
const EnumDyeColor& EnumDyeColor::GRAY() {
    static const EnumDyeColor instance(7, 8, "gray", "gray", 4673362, TextFormatting::DARK_GRAY());
    return instance;
}
const EnumDyeColor& EnumDyeColor::SILVER() {
    static const EnumDyeColor instance(8, 7, "silver", "silver", 10329495, TextFormatting::GRAY());
    return instance;
}
const EnumDyeColor& EnumDyeColor::CYAN() {
    static const EnumDyeColor instance(9, 6, "cyan", "cyan", 1481884, TextFormatting::DARK_AQUA());
    return instance;
}
const EnumDyeColor& EnumDyeColor::PURPLE() {
    static const EnumDyeColor instance(10, 5, "purple", "purple", 8991416, TextFormatting::DARK_PURPLE());
    return instance;
}
const EnumDyeColor& EnumDyeColor::BLUE() {
    static const EnumDyeColor instance(11, 4, "blue", "blue", 3949738, TextFormatting::DARK_BLUE());
    return instance;
}
const EnumDyeColor& EnumDyeColor::BROWN() {
    static const EnumDyeColor instance(12, 3, "brown", "brown", 8606770, TextFormatting::GOLD());
    return instance;
}
const EnumDyeColor& EnumDyeColor::GREEN() {
    static const EnumDyeColor instance(13, 2, "green", "green", 6192150, TextFormatting::DARK_GREEN());
    return instance;
}
const EnumDyeColor& EnumDyeColor::RED() {
    static const EnumDyeColor instance(14, 1, "red", "red", 11546150, TextFormatting::DARK_RED());
    return instance;
}
const EnumDyeColor& EnumDyeColor::BLACK() {
    static const EnumDyeColor instance(15, 0, "black", "black", 1908001, TextFormatting::BLACK());
    return instance;
}

// ---------------------------------------------------------------------------
// Lookup tables
// ---------------------------------------------------------------------------
const EnumDyeColor* EnumDyeColor::s_metaLookup[16]     = {};
const EnumDyeColor* EnumDyeColor::s_dyeDamageLookup[16] = {};
bool EnumDyeColor::s_lookupsInitialized                 = false;

void EnumDyeColor::initLookups() {
    if (s_lookupsInitialized)
        return;

    // Calling each accessor here ensures the instances are alive before we
    // store their addresses — no dangling pointer risk.
    const EnumDyeColor* all[16] = {
        &WHITE(),  &ORANGE(), &MAGENTA(), &LIGHT_BLUE(), &YELLOW(), &LIME(),  &PINK(),  &GRAY(),
        &SILVER(), &CYAN(),   &PURPLE(),  &BLUE(),       &BROWN(),  &GREEN(), &RED(),   &BLACK()
    };

    for (const EnumDyeColor* color : all) {
        s_metaLookup[color->m_meta]           = color;
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

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
EnumDyeColor::EnumDyeColor(int meta, int dyeDamage, const char* name, const char* unlocalizedName, int colorValue,
                           const TextFormatting& chatColor) :
    m_meta(meta),
    m_dyeDamage(dyeDamage),
    m_name(name),
    m_unlocalizedName(unlocalizedName),
    m_colorValue(colorValue),
    m_chatColor(&chatColor) // store address of the singleton reference
{
    int r = (colorValue & 0xFF0000) >> 16;
    int g = (colorValue & 0x00FF00) >> 8;
    int b = (colorValue & 0x0000FF);
    m_colorComponents = {r / 255.0f, g / 255.0f, b / 255.0f};
}

// ---------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------
int                        EnumDyeColor::getMetadata()            const { return m_meta; }
int                        EnumDyeColor::getDyeDamage()           const { return m_dyeDamage; }
int                        EnumDyeColor::getColorValue()          const { return m_colorValue; }
const std::string&         EnumDyeColor::getName()                const { return m_name; }
const std::string&         EnumDyeColor::getUnlocalizedName()     const { return m_unlocalizedName; }
const std::array<float,3>& EnumDyeColor::getColorComponentValues() const { return m_colorComponents; }
const TextFormatting&      EnumDyeColor::getChatColor()           const { return *m_chatColor; }

// IStringSerializable
const std::string& EnumDyeColor::getSerializedName() const { return m_name; }

std::string EnumDyeColor::toString() const { return m_unlocalizedName; }

bool EnumDyeColor::operator==(const EnumDyeColor& other) const { return m_meta == other.m_meta; }
bool EnumDyeColor::operator!=(const EnumDyeColor& other) const { return !(*this == other); }