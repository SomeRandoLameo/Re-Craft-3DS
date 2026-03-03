#pragma once

#include "internal/json.hpp"

class ResourceLocation {
public:
    explicit ResourceLocation(const std::string& resourceName);
    ResourceLocation(const std::string& domain, const std::string& path);

    const std::string& getResourcePath() const;
    const std::string& getResourceDomain() const;

    std::string toString() const;

    bool operator==(const ResourceLocation& other) const;
    bool operator!=(const ResourceLocation& other) const;
    bool operator<(const ResourceLocation& other) const;

    struct Hash {
        std::size_t operator()(const ResourceLocation& rl) const;
    };

protected:
    std::string m_domain;
    std::string m_path;

    /**
     * Splits an object name (such as minecraft:apple) into the domain and path parts and returns these as an array of
     * length 2. If no colon is present in the passed value the returned array will contain {null, toSplit}.
     */
    static std::pair<std::string, std::string> splitObjectName(const std::string& toSplit);

private:
    void init(const std::string& domain, const std::string& path);
};

void to_json(nlohmann::json& j, const ResourceLocation& rl);
void from_json(const nlohmann::json& j, ResourceLocation& rl);
