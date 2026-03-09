#pragma once

#include <array>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

class TextFormatting {
public:
    // Color codes
    static const TextFormatting& BLACK();
    static const TextFormatting& DARK_BLUE();
    static const TextFormatting& DARK_GREEN();
    static const TextFormatting& DARK_AQUA();
    static const TextFormatting& DARK_RED();
    static const TextFormatting& DARK_PURPLE();
    static const TextFormatting& GOLD();
    static const TextFormatting& GRAY();
    static const TextFormatting& DARK_GRAY();
    static const TextFormatting& BLUE();
    static const TextFormatting& GREEN();
    static const TextFormatting& AQUA();
    static const TextFormatting& RED();
    static const TextFormatting& LIGHT_PURPLE();
    static const TextFormatting& YELLOW();
    static const TextFormatting& WHITE();

    // Fancy styling codes
    static const TextFormatting& OBFUSCATED();
    static const TextFormatting& BOLD();
    static const TextFormatting& STRIKETHROUGH();
    static const TextFormatting& UNDERLINE();
    static const TextFormatting& ITALIC();
    static const TextFormatting& RESET();

    int getColorIndex() const;
    bool isFancyStyling() const;
    bool isColor() const;
    std::string getFriendlyName() const;
    std::string toString() const;

    static std::optional<std::string> getTextWithoutFormattingCodes(const std::string* text);
    static const TextFormatting* getValueByName(const std::string* friendlyName);
    static const TextFormatting* fromColorIndex(int index);
    static std::vector<std::string> getValidValues(bool includeColors, bool includeFancyStyling);

    bool operator==(const TextFormatting& other) const;
    bool operator!=(const TextFormatting& other) const;

private:
    TextFormatting(const char* name, char formattingCode, int colorIndex);
    TextFormatting(const char* name, char formattingCode, bool fancyStyling);
    TextFormatting(const char* name, char formattingCode, bool fancyStyling, int colorIndex);

    static std::string lowercaseAlpha(const std::string& input);
    static void initNameMapping();

    static const std::array<const TextFormatting*, 22>& allValues();

    static const std::regex& formattingCodePattern();

    std::string m_name;
    char m_formattingCode;
    bool m_fancyStyling;
    int m_colorIndex;
    std::string m_controlString;

    static std::unordered_map<std::string, const TextFormatting*> s_nameMapping;
    static bool s_initialized;
};