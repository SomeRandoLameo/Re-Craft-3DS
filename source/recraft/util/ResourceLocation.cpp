#include "util/ResourceLocation.hpp"

#include <algorithm>
#include <stdexcept>

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

void ResourceLocation::init(const std::string& domain, const std::string& path) {
    m_domain = domain.empty() ? "minecraft" : toLower(domain);
    m_path = toLower(path);

    if (m_path.empty())
        throw std::invalid_argument("ResourceLocation path must not be null/empty");
}

ResourceLocation::ResourceLocation(const std::string& resourceName) {
    auto [domain, path] = splitObjectName(resourceName);
    init(domain, path);
}

ResourceLocation::ResourceLocation(const std::string& domain, const std::string& path) { init(domain, path); }

std::pair<std::string, std::string> ResourceLocation::splitObjectName(const std::string& toSplit) {
    auto pos = toSplit.find(':');

    if (pos == std::string::npos)
        return {"minecraft", toSplit};

    std::string domain = (pos > 0) ? toSplit.substr(0, pos) : "minecraft";
    std::string path = toSplit.substr(pos + 1);
    return {domain, path};
}

const std::string& ResourceLocation::getResourcePath() const { return m_path; }
const std::string& ResourceLocation::getResourceDomain() const { return m_domain; }

std::string ResourceLocation::toString() const { return m_domain + ':' + m_path; }

bool ResourceLocation::operator==(const ResourceLocation& o) const {
    return m_domain == o.m_domain && m_path == o.m_path;
}

bool ResourceLocation::operator!=(const ResourceLocation& o) const { return !(*this == o); }

bool ResourceLocation::operator<(const ResourceLocation& o) const {
    if (m_domain != o.m_domain)
        return m_domain < o.m_domain;
    return m_path < o.m_path;
}

std::size_t ResourceLocation::Hash::operator()(const ResourceLocation& rl) const {
    std::size_t h1 = std::hash<std::string>{}(rl.m_domain);
    std::size_t h2 = std::hash<std::string>{}(rl.m_path);
    return h1 * 31 + h2;
}

void to_json(nlohmann::json& j, const ResourceLocation& rl) { j = rl.toString(); }

void from_json(const nlohmann::json& j, ResourceLocation& rl) { rl = ResourceLocation(j.get<std::string>()); }
