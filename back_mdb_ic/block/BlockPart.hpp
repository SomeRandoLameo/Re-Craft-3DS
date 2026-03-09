#pragma once

#include <array>
#include <map>
#include <optional>
#include <string_view>
#include "BlockPartFace.hpp"
#include "BlockPartRotation.hpp"
#include "internal/json.hpp"

class BlockPart {
public:
    mc::Vector3f positionFrom;
    mc::Vector3f positionTo;
    std::map<EnumFacing::Value, BlockPartFace> mapFaces;
    std::optional<BlockPartRotation> partRotation;
    bool shade;

    BlockPart(const mc::Vector3f& positionFromIn, const mc::Vector3f& positionToIn,
              std::map<EnumFacing::Value, BlockPartFace> mapFacesIn, std::optional<BlockPartRotation> partRotationIn,
              bool shadeIn);

    BlockPart() : shade(true) {}

    static void from_json(const nlohmann::json& j, BlockPart& b);

private:
    void SetDefaultUvs();
    std::array<float, 4> GetFaceUvs(EnumFacing::Value facing) const;

    static std::map<EnumFacing::Value, BlockPartFace> parseFaces(const nlohmann::json& j);
    static mc::Vector3f parsePosition(const nlohmann::json& j, std::string_view member);
    static mc::Vector3f parsePositionBounded(const nlohmann::json& j, std::string_view member);
    static EnumFacing::Axis::Value parseAxis(const nlohmann::json& j);
    static float parseAngle(const nlohmann::json& j);
    static std::optional<BlockPartRotation> parseRotation(const nlohmann::json& j);
    static EnumFacing::Value parseEnumFacing(const std::string& name);
};
