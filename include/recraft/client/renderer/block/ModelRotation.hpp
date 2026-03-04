#pragma once

#include <amethyst/maths/mat.hpp>
#include <optional>
#include <unordered_map>
#include "util/EnumFacing.hpp"

class ModelRotation {
public:
    enum class Value {
        X0_Y0,
        X0_Y90,
        X0_Y180,
        X0_Y270,
        X90_Y0,
        X90_Y90,
        X90_Y180,
        X90_Y270,
        X180_Y0,
        X180_Y90,
        X180_Y180,
        X180_Y270,
        X270_Y0,
        X270_Y90,
        X270_Y180,
        X270_Y270,
        COUNT
    };

    explicit ModelRotation(Value value);

    /**
     * Returns the ModelRotation for the given x/y angles (in degrees).
     * Angles are normalised to [0, 360) before lookup.
     * Returns std::nullopt if the combination is not a valid rotation.
     */
    static std::optional<ModelRotation> getModelRotation(int x, int y);

    const Amy::mat4& getMatrix4d() const;
    int getX() const;
    int getY() const;
    Value getValue() const;

    EnumFacing rotateFace(EnumFacing facing) const;
    int rotateVertex(EnumFacing facing, int vertexIndex) const;

    bool operator==(const ModelRotation& other) const;
    bool operator!=(const ModelRotation& other) const;
    std::size_t hashCode() const;
    std::string toString() const;

private:
    Value m_value;
    int m_x;
    int m_y;
    int m_quartersX;
    int m_quartersY;
    Amy::mat4 m_matrix;

    static const std::array<ModelRotation, static_cast<std::size_t>(Value::COUNT)>& allValues();
    static const std::unordered_map<int, Value>& rotationMap();

    static int combineXY(int x, int y);
    static int normalizeAngle(int angle);
};
