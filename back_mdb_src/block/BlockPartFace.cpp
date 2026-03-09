#include "client/renderer/block/BlockPartFace.hpp"

BlockPartFace::BlockPartFace(std::optional<EnumFacing::Value> cullFaceIn, int tintIndexIn, std::string textureIn,
                             BlockFaceUV blockFaceUVIn) :
    cullFace(cullFaceIn),
    tintIndex(tintIndexIn),
    texture(std::move(textureIn)),
    blockFaceUV(std::move(blockFaceUVIn)) {}

std::optional<EnumFacing::Value> BlockPartFace::parseCullFace(const nlohmann::json& j) {
    auto it = j.find("cullface");
    if (it == j.end() || !it->is_string())
        return std::nullopt;
    return EnumFacing::byName(it->get_ref<const std::string&>());
}

int BlockPartFace::parseTintIndex(const nlohmann::json& j) { return j.value("tintindex", -1); }

std::string BlockPartFace::parseTexture(const nlohmann::json& j) {
    return j.at("texture").get<std::string>();
}

BlockFaceUV BlockPartFace::parseBlockFaceUV(const nlohmann::json& j) {
    BlockFaceUV uv;
    BlockFaceUV::from_json(j, uv);
    return uv;
}

void BlockPartFace::from_json(const nlohmann::json& j, BlockPartFace& b) {
    b.cullFace    = parseCullFace(j);
    b.tintIndex   = parseTintIndex(j);
    b.texture     = parseTexture(j);
    BlockFaceUV::from_json(j, b.blockFaceUV);
}
