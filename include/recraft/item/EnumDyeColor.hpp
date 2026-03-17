#pragma once

#include <array>
#include <string>
#include "util/text/TextFormatting.hpp"


class EnumDyeColor {
public:
    static const EnumDyeColor& WHITE();
    static const EnumDyeColor& ORANGE();
    static const EnumDyeColor& MAGENTA();
    static const EnumDyeColor& LIGHT_BLUE();
    static const EnumDyeColor& YELLOW();
    static const EnumDyeColor& LIME();
    static const EnumDyeColor& PINK();
    static const EnumDyeColor& GRAY();
    static const EnumDyeColor& SILVER();
    static const EnumDyeColor& CYAN();
    static const EnumDyeColor& PURPLE();
    static const EnumDyeColor& BLUE();
    static const EnumDyeColor& BROWN();
    static const EnumDyeColor& GREEN();
    static const EnumDyeColor& RED();
    static const EnumDyeColor& BLACK();

    static const EnumDyeColor& byMetadata(int meta);
    static const EnumDyeColor& byDyeDamage(int damage);

    int getMetadata() const;
    int getDyeDamage() const;
    int getColorValue() const;
    const std::string& getName() const;
    const std::string& getUnlocalizedName() const;
    const std::array<float, 3>& getColorComponentValues() const;
    const TextFormatting& getChatColor() const;

    std::string toString() const;

    // IStringSerializable
    const std::string& getSerializedName() const;

    bool operator==(const EnumDyeColor& other) const;
    bool operator!=(const EnumDyeColor& other) const;

private:
    EnumDyeColor(int meta, int dyeDamage, const char* name, const char* unlocalizedName, int colorValue,
                 const TextFormatting& chatColor);

    static void initLookups();

    int m_meta;
    int m_dyeDamage;
    std::string m_name;
    std::string m_unlocalizedName;
    int m_colorValue;
    std::array<float, 3> m_colorComponents;
    const TextFormatting* m_chatColor; // pointer to avoid copying a non-copyable singleton

    static const EnumDyeColor* s_metaLookup[16];
    static const EnumDyeColor* s_dyeDamageLookup[16];
    static bool s_lookupsInitialized;
};
