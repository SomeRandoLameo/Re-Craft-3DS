#include "util/text/TextFormatting.hpp"

#include <algorithm>
#include <cctype>

const TextFormatting TextFormatting::BLACK("BLACK", '0', false, 0);
const TextFormatting TextFormatting::DARK_BLUE("DARK_BLUE", '1', false, 1);
const TextFormatting TextFormatting::DARK_GREEN("DARK_GREEN", '2', false, 2);
const TextFormatting TextFormatting::DARK_AQUA("DARK_AQUA", '3', false, 3);
const TextFormatting TextFormatting::DARK_RED("DARK_RED", '4', false, 4);
const TextFormatting TextFormatting::DARK_PURPLE("DARK_PURPLE", '5', false, 5);
const TextFormatting TextFormatting::GOLD("GOLD", '6', false, 6);
const TextFormatting TextFormatting::GRAY("GRAY", '7', false, 7);
const TextFormatting TextFormatting::DARK_GRAY("DARK_GRAY", '8', false, 8);
const TextFormatting TextFormatting::BLUE("BLUE", '9', false, 9);
const TextFormatting TextFormatting::GREEN("GREEN", 'a', false, 10);
const TextFormatting TextFormatting::AQUA("AQUA", 'b', false, 11);
const TextFormatting TextFormatting::RED("RED", 'c', false, 12);
const TextFormatting TextFormatting::LIGHT_PURPLE("LIGHT_PURPLE", 'd', false, 13);
const TextFormatting TextFormatting::YELLOW("YELLOW", 'e', false, 14);
const TextFormatting TextFormatting::WHITE("WHITE", 'f', false, 15);
const TextFormatting TextFormatting::OBFUSCATED("OBFUSCATED", 'k', true, -1);
const TextFormatting TextFormatting::BOLD("BOLD", 'l', true, -1);
const TextFormatting TextFormatting::STRIKETHROUGH("STRIKETHROUGH", 'm', true, -1);
const TextFormatting TextFormatting::UNDERLINE("UNDERLINE", 'n', true, -1);
const TextFormatting TextFormatting::ITALIC("ITALIC", 'o', true, -1);
const TextFormatting TextFormatting::RESET("RESET", 'r', false, -1);

const TextFormatting* TextFormatting::s_allValues[22] = {
    &BLACK,      &DARK_BLUE, &DARK_GREEN,    &DARK_AQUA, &DARK_RED, &DARK_PURPLE,  &GOLD,   &GRAY,
    &DARK_GRAY,  &BLUE,      &GREEN,         &AQUA,      &RED,      &LIGHT_PURPLE, &YELLOW, &WHITE,
    &OBFUSCATED, &BOLD,      &STRIKETHROUGH, &UNDERLINE, &ITALIC,   &RESET};

std::unordered_map<std::string, const TextFormatting*> TextFormatting::s_nameMapping;
bool TextFormatting::s_initialized = false;

// §  is U+00A7, encoded as this shit in UTF-8
const std::regex TextFormatting::s_formattingCodePattern("\xC2\xA7[0-9A-FK-ORa-fk-or]", std::regex::icase);

TextFormatting::TextFormatting(const char* name, char formattingCode, int colorIndex) :
    TextFormatting(name, formattingCode, false, colorIndex) {}

TextFormatting::TextFormatting(const char* name, char formattingCode, bool fancyStyling) :
    TextFormatting(name, formattingCode, fancyStyling, -1) {}

TextFormatting::TextFormatting(const char* name, char formattingCode, bool fancyStyling, int colorIndex) :
    m_name(name), m_formattingCode(formattingCode), m_fancyStyling(fancyStyling), m_colorIndex(colorIndex),
    m_controlString(std::string("\xC2\xA7") + formattingCode) // UTF-8 section sign
{}

std::string TextFormatting::lowercaseAlpha(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    for (char c : input) {
        char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (lower >= 'a' && lower <= 'z')
            result += lower;
    }
    return result;
}

void TextFormatting::initNameMapping() {
    if (s_initialized)
        return;
    for (const TextFormatting* tf : s_allValues)
        s_nameMapping[lowercaseAlpha(tf->m_name)] = tf;
    s_initialized = true;
}

int TextFormatting::getColorIndex() const { return m_colorIndex; }
bool TextFormatting::isFancyStyling() const { return m_fancyStyling; }

bool TextFormatting::isColor() const { return !m_fancyStyling && (this != &RESET); }

std::string TextFormatting::getFriendlyName() const {
    std::string result = m_name;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::string TextFormatting::toString() const { return m_controlString; }

std::optional<std::string> TextFormatting::getTextWithoutFormattingCodes(const std::string* text) {
    if (!text)
        return std::nullopt;
    return std::regex_replace(*text, s_formattingCodePattern, "");
}

const TextFormatting* TextFormatting::getValueByName(const std::string* friendlyName) {
    if (!friendlyName)
        return nullptr;
    initNameMapping();
    auto it = s_nameMapping.find(lowercaseAlpha(*friendlyName));
    return it != s_nameMapping.end() ? it->second : nullptr;
}

const TextFormatting* TextFormatting::fromColorIndex(int index) {
    if (index < 0)
        return &RESET;
    for (const TextFormatting* tf : s_allValues) {
        if (tf->getColorIndex() == index)
            return tf;
    }
    return nullptr;
}

std::vector<std::string> TextFormatting::getValidValues(bool includeColors, bool includeFancyStyling) {
    std::vector<std::string> result;
    for (const TextFormatting* tf : s_allValues) {
        if ((!tf->isColor() || includeColors) && (!tf->isFancyStyling() || includeFancyStyling))
            result.push_back(tf->getFriendlyName());
    }
    return result;
}

bool TextFormatting::operator==(const TextFormatting& other) const { return this == &other; }
bool TextFormatting::operator!=(const TextFormatting& other) const { return !(*this == other); }
