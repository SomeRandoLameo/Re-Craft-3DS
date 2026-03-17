#include "util/text/TextFormatting.hpp"

#include <algorithm>
#include <cctype>


const TextFormatting& TextFormatting::BLACK() {
    static const TextFormatting instance("BLACK", '0', false, 0);
    return instance;
}
const TextFormatting& TextFormatting::DARK_BLUE() {
    static const TextFormatting instance("DARK_BLUE", '1', false, 1);
    return instance;
}
const TextFormatting& TextFormatting::DARK_GREEN() {
    static const TextFormatting instance("DARK_GREEN", '2', false, 2);
    return instance;
}
const TextFormatting& TextFormatting::DARK_AQUA() {
    static const TextFormatting instance("DARK_AQUA", '3', false, 3);
    return instance;
}
const TextFormatting& TextFormatting::DARK_RED() {
    static const TextFormatting instance("DARK_RED", '4', false, 4);
    return instance;
}
const TextFormatting& TextFormatting::DARK_PURPLE() {
    static const TextFormatting instance("DARK_PURPLE", '5', false, 5);
    return instance;
}
const TextFormatting& TextFormatting::GOLD() {
    static const TextFormatting instance("GOLD", '6', false, 6);
    return instance;
}
const TextFormatting& TextFormatting::GRAY() {
    static const TextFormatting instance("GRAY", '7', false, 7);
    return instance;
}
const TextFormatting& TextFormatting::DARK_GRAY() {
    static const TextFormatting instance("DARK_GRAY", '8', false, 8);
    return instance;
}
const TextFormatting& TextFormatting::BLUE() {
    static const TextFormatting instance("BLUE", '9', false, 9);
    return instance;
}
const TextFormatting& TextFormatting::GREEN() {
    static const TextFormatting instance("GREEN", 'a', false, 10);
    return instance;
}
const TextFormatting& TextFormatting::AQUA() {
    static const TextFormatting instance("AQUA", 'b', false, 11);
    return instance;
}
const TextFormatting& TextFormatting::RED() {
    static const TextFormatting instance("RED", 'c', false, 12);
    return instance;
}
const TextFormatting& TextFormatting::LIGHT_PURPLE() {
    static const TextFormatting instance("LIGHT_PURPLE", 'd', false, 13);
    return instance;
}
const TextFormatting& TextFormatting::YELLOW() {
    static const TextFormatting instance("YELLOW", 'e', false, 14);
    return instance;
}
const TextFormatting& TextFormatting::WHITE() {
    static const TextFormatting instance("WHITE", 'f', false, 15);
    return instance;
}
const TextFormatting& TextFormatting::OBFUSCATED() {
    static const TextFormatting instance("OBFUSCATED", 'k', true, -1);
    return instance;
}
const TextFormatting& TextFormatting::BOLD() {
    static const TextFormatting instance("BOLD", 'l', true, -1);
    return instance;
}
const TextFormatting& TextFormatting::STRIKETHROUGH() {
    static const TextFormatting instance("STRIKETHROUGH", 'm', true, -1);
    return instance;
}
const TextFormatting& TextFormatting::UNDERLINE() {
    static const TextFormatting instance("UNDERLINE", 'n', true, -1);
    return instance;
}
const TextFormatting& TextFormatting::ITALIC() {
    static const TextFormatting instance("ITALIC", 'o', true, -1);
    return instance;
}
const TextFormatting& TextFormatting::RESET() {
    static const TextFormatting instance("RESET", 'r', false, -1);
    return instance;
}

const std::array<const TextFormatting*, 22>& TextFormatting::allValues() {
    static const std::array<const TextFormatting*, 22> values = {
        &BLACK(),      &DARK_BLUE(), &DARK_GREEN(),    &DARK_AQUA(), &DARK_RED(), &DARK_PURPLE(),  &GOLD(),   &GRAY(),
        &DARK_GRAY(),  &BLUE(),      &GREEN(),         &AQUA(),      &RED(),      &LIGHT_PURPLE(), &YELLOW(), &WHITE(),
        &OBFUSCATED(), &BOLD(),      &STRIKETHROUGH(), &UNDERLINE(), &ITALIC(),   &RESET()};
    return values;
}


const std::regex& TextFormatting::formattingCodePattern() {
    static const std::regex pattern("\xC2\xA7[0-9A-FK-ORa-fk-or]", std::regex::icase);
    return pattern;
}

std::unordered_map<std::string, const TextFormatting*> TextFormatting::s_nameMapping;
bool TextFormatting::s_initialized = false;

TextFormatting::TextFormatting(const char* name, char formattingCode, int colorIndex) :
    TextFormatting(name, formattingCode, false, colorIndex) {}

TextFormatting::TextFormatting(const char* name, char formattingCode, bool fancyStyling) :
    TextFormatting(name, formattingCode, fancyStyling, -1) {}

TextFormatting::TextFormatting(const char* name, char formattingCode, bool fancyStyling, int colorIndex) :
    m_name(name), m_formattingCode(formattingCode), m_fancyStyling(fancyStyling), m_colorIndex(colorIndex),
    m_controlString(std::string("\xC2\xA7") + formattingCode) {}

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
    for (const TextFormatting* tf : allValues())
        s_nameMapping[lowercaseAlpha(tf->m_name)] = tf;
    s_initialized = true;
}

int TextFormatting::getColorIndex() const { return m_colorIndex; }
bool TextFormatting::isFancyStyling() const { return m_fancyStyling; }
bool TextFormatting::isColor() const { return !m_fancyStyling && (this != &RESET()); }

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
    return std::regex_replace(*text, formattingCodePattern(), "");
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
        return &RESET();
    for (const TextFormatting* tf : allValues()) {
        if (tf->getColorIndex() == index)
            return tf;
    }
    return nullptr;
}

std::vector<std::string> TextFormatting::getValidValues(bool includeColors, bool includeFancyStyling) {
    std::vector<std::string> result;
    for (const TextFormatting* tf : allValues()) {
        if ((!tf->isColor() || includeColors) && (!tf->isFancyStyling() || includeFancyStyling))
            result.push_back(tf->getFriendlyName());
    }
    return result;
}

bool TextFormatting::operator==(const TextFormatting& other) const { return this == &other; }
bool TextFormatting::operator!=(const TextFormatting& other) const { return !(*this == other); }
