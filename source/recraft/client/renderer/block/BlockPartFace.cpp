#include "client/renderer/block/BlockPartFace.hpp"

BlockPartFace::BlockPartFace(std::optional<EnumFacing::Value> cullFaceIn, int tintIndexIn, std::string textureIn,
                             BlockFaceUV blockFaceUVIn) :
    cullFace(cullFaceIn), tintIndex(tintIndexIn), texture(std::move(textureIn)), blockFaceUV(std::move(blockFaceUVIn)) {
}

std::optional<EnumFacing::Value> BlockPartFace::parseCullFace(const nlohmann::json& j) {
    const auto& s = j.value("cullface", std::string{});
    if (s.empty())
        return std::nullopt;
    return EnumFacing::byName(s);
}

int BlockPartFace::parseTintIndex(const nlohmann::json& j) { return j.value("tintindex", -1); }

std::string BlockPartFace::parseTexture(const nlohmann::json& j) { return j.at("texture").get<std::string>(); }

BlockFaceUV BlockPartFace::parseBlockFaceUV(const nlohmann::json& j) {
    BlockFaceUV uv;
    BlockFaceUV::from_json(j, uv);
    return uv;
}

void BlockPartFace::from_json(const nlohmann::json& j, BlockPartFace& b) {
    b = BlockPartFace(parseCullFace(j), parseTintIndex(j), parseTexture(j), parseBlockFaceUV(j));
}
