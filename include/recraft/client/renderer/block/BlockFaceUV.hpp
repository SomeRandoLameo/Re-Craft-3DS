#pragma once

#include <array>
#include <optional>
#include "internal/json.hpp"

class BlockFaceUV {
public:
    std::optional<std::array<float, 4>> uvs;
    int rotation;

    BlockFaceUV() : rotation(0) {}
    BlockFaceUV(std::optional<std::array<float, 4>> uvsIn, int rotationIn);

    float GetVertexU(int vertex) const;
    float GetVertexV(int vertex) const;
    int GetVertexRotatedRev(int vertex) const;

    void SetUvs(const std::array<float, 4>& uvsIn);

    static void from_json(const nlohmann::json& j, BlockFaceUV& b);

private:
    int GetVertexRotated(int vertex) const;

    static int parseRotation(const nlohmann::json& j);
    static std::optional<std::array<float, 4>> parseUV(const nlohmann::json& j);
};
