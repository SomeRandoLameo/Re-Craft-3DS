#pragma once

#include <optional>
#include <string>
#include "BlockFaceUV.hpp"
#include "util/EnumFacing.hpp"


class BlockPartFace {
public:
    static constexpr std::nullopt_t FACING_DEFAULT = std::nullopt;

    std::optional<EnumFacing::Value> cullFace;
    int tintIndex;
    std::string texture;
    BlockFaceUV blockFaceUV;

    BlockPartFace() : cullFace(std::nullopt), tintIndex(-1), texture(""), blockFaceUV() {}
    BlockPartFace(std::optional<EnumFacing::Value> cullFaceIn, int tintIndexIn, std::string textureIn,
                  BlockFaceUV blockFaceUVIn);

    static void from_json(const nlohmann::json& j, BlockPartFace& b);

private:
    static BlockFaceUV parseBlockFaceUV(const nlohmann::json& j);
    static std::string parseTexture(const nlohmann::json& j);
    static int parseTintIndex(const nlohmann::json& j);
    static std::optional<EnumFacing::Value> parseCullFace(const nlohmann::json& j);
};
