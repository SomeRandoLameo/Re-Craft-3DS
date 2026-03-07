#include "client/renderer/block/BlockFaceUV.hpp"

BlockFaceUV::BlockFaceUV(std::optional<std::array<float, 4>> uvsIn, int rotationIn) :
    uvs(uvsIn), rotation(rotationIn) {}

int BlockFaceUV::GetVertexRotated(int vertex) const { return (vertex + rotation / 90) % 4; }

float BlockFaceUV::GetVertexU(int vertex) const {
    if (!uvs.has_value())
        throw std::runtime_error("uvs is null");

    int i = GetVertexRotated(vertex);
    return (i != 0 && i != 1) ? uvs->at(2) : uvs->at(0);
}

float BlockFaceUV::GetVertexV(int vertex) const {
    if (!uvs.has_value())
        throw std::runtime_error("uvs is null");

    int i = GetVertexRotated(vertex);
    return (i != 0 && i != 3) ? uvs->at(3) : uvs->at(1);
}

int BlockFaceUV::GetVertexRotatedRev(int vertex) const { return (vertex + (4 - rotation / 90)) % 4; }

void BlockFaceUV::SetUvs(const std::array<float, 4>& uvsIn) {
    if (!uvs.has_value())
        uvs.emplace(uvsIn);
}

int BlockFaceUV::parseRotation(const nlohmann::json& j) {
    int rot = j.value("rotation", 0);

    if (rot >= 0 && rot % 90 == 0 && rot / 90 <= 3)
        return rot;

    throw nlohmann::json::parse_error::create(
        101, 0, "Invalid rotation " + std::to_string(rot) + " found, only 0/90/180/270 allowed", nullptr);
}

std::optional<std::array<float, 4>> BlockFaceUV::parseUV(const nlohmann::json& j) {
    if (!j.contains("uv"))
        return std::nullopt;

    const auto& arr = j.at("uv");

    if (!arr.is_array() || arr.size() != 4) {
        throw nlohmann::json::parse_error::create(101, 0, "Expected 4 uv values, found: " + std::to_string(arr.size()),
                                                  nullptr);
    }

    std::array<float, 4> uvs{};
    for (int i = 0; i < 4; ++i) {
        if (!arr[i].is_number()) {
            throw nlohmann::json::parse_error::create(101, 0, "uv[" + std::to_string(i) + "] is not a number", nullptr);
        }
        uvs[i] = arr[i].get<float>();
    }

    return uvs;
}

void BlockFaceUV::from_json(const nlohmann::json& j, BlockFaceUV& b) { b = BlockFaceUV(parseUV(j), parseRotation(j)); }
