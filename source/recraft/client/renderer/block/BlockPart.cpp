#include "client/renderer/block/BlockPart.hpp"
#include "util/EnumFacing.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

// ---- Constructor ------------------------------------------------------------

BlockPart::BlockPart(mc::Vector3f positionFromIn, mc::Vector3f positionToIn,
                     std::map<EnumFacing::Value, BlockPartFace> mapFacesIn,
                     std::optional<BlockPartRotation> partRotationIn, bool shadeIn) :
    positionFrom(positionFromIn), positionTo(positionToIn), mapFaces(std::move(mapFacesIn)),
    partRotation(partRotationIn), shade(shadeIn) {
    SetDefaultUvs();
}

// ---- UV helpers -------------------------------------------------------------

void BlockPart::SetDefaultUvs() {
    for (auto& [facing, face] : mapFaces)
        face.blockFaceUV.SetUvs(GetFaceUvs(facing));
}

std::array<float, 4> BlockPart::GetFaceUvs(EnumFacing::Value facing) const {
    switch (facing) {
    case EnumFacing::DOWN:
        return {positionFrom.x, 16.0f - positionTo.z, positionTo.x, 16.0f - positionFrom.z};
    case EnumFacing::UP:
        return {positionFrom.x, positionFrom.z, positionTo.x, positionTo.z};
    case EnumFacing::SOUTH:
        return {positionFrom.x, 16.0f - positionTo.y, positionTo.x, 16.0f - positionFrom.y};
    case EnumFacing::WEST:
        return {positionFrom.z, 16.0f - positionTo.y, positionTo.z, 16.0f - positionFrom.y};
    case EnumFacing::EAST:
        return {16.0f - positionTo.z, 16.0f - positionTo.y, 16.0f - positionFrom.z, 16.0f - positionFrom.y};
    case EnumFacing::NORTH:
    default:
        return {16.0f - positionTo.x, 16.0f - positionTo.y, 16.0f - positionFrom.x, 16.0f - positionFrom.y};
    }
}


mc::Vector3f BlockPart::parsePosition(const nlohmann::json& j, const std::string& member) {
    const auto& arr = j.at(member);

    if (!arr.is_array() || arr.size() != 3) {
        throw nlohmann::json::parse_error::create(
            101, 0, "Expected 3 " + member + " values, found: " + std::to_string(arr.size()), nullptr);
    }

    return {arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>()};
}

mc::Vector3f BlockPart::parsePositionBounded(const nlohmann::json& j, const std::string& member) {
    mc::Vector3f v = parsePosition(j, member);

    if (v.x < -16.0f || v.y < -16.0f || v.z < -16.0f || v.x > 32.0f || v.y > 32.0f || v.z > 32.0f) {
        throw nlohmann::json::parse_error::create(101, 0, "'" + member + "' specifier exceeds the allowed boundaries",
                                                  nullptr);
    }

    return v;
}

EnumFacing::Axis::Value BlockPart::parseAxis(const nlohmann::json& j) {
    std::string s = j.at("axis").get<std::string>();
    auto axis = EnumFacing::Axis::byName(s);

    if (!axis) {
        throw nlohmann::json::parse_error::create(101, 0, "Invalid rotation axis: " + s, nullptr);
    }

    return *axis;
}

float BlockPart::parseAngle(const nlohmann::json& j) {
    float f = j.at("angle").get<float>();

    if (f != 0.0f && std::abs(f) != 22.5f && std::abs(f) != 45.0f) {
        throw nlohmann::json::parse_error::create(
            101, 0, "Invalid rotation " + std::to_string(f) + " found, only -45/-22.5/0/22.5/45 allowed", nullptr);
    }

    return f;
}

std::optional<BlockPartRotation> BlockPart::parseRotation(const nlohmann::json& j) {
    if (!j.contains("rotation"))
        return std::nullopt;

    const auto& rot = j.at("rotation");
    mc::Vector3f origin = parsePosition(rot, "origin");
    origin *= 0.0625f;

    return BlockPartRotation(origin, parseAxis(rot), parseAngle(rot), rot.value("rescale", false));
}

EnumFacing::Value BlockPart::parseEnumFacing(const std::string& name) {
    auto facing = EnumFacing::byName(name);

    if (!facing) {
        throw nlohmann::json::parse_error::create(101, 0, "Unknown facing: " + name, nullptr);
    }

    return *facing;
}

std::map<EnumFacing::Value, BlockPartFace> BlockPart::parseFaces(const nlohmann::json& j) {
    std::map<EnumFacing::Value, BlockPartFace> map;
    const auto& faces = j.at("faces");

    for (auto& [key, val] : faces.items()) {
        BlockPartFace face;
        BlockPartFace::from_json(val, face);
        map.emplace(parseEnumFacing(key), std::move(face));
    }

    if (map.empty()) {
        throw nlohmann::json::parse_error::create(101, 0, "Expected between 1 and 6 unique faces, got 0", nullptr);
    }

    return map;
}

void BlockPart::from_json(const nlohmann::json& j, BlockPart& b) {
    bool hasShade = true;

    if (j.contains("shade")) {
        if (!j.at("shade").is_boolean()) {
            throw nlohmann::json::parse_error::create(101, 0, "Expected shade to be a Boolean", nullptr);
        }
        hasShade = j.at("shade").get<bool>();
    }

    b = BlockPart(parsePositionBounded(j, "from"), parsePositionBounded(j, "to"), parseFaces(j), parseRotation(j),
                  hasShade);
}
