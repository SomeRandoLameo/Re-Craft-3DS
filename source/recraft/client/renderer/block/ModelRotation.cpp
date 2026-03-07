#include "client/renderer/block/ModelRotation.hpp"

#include <cmath>
#include <stdexcept>


int ModelRotation::combineXY(int x, int y) { return x * 360 + y; }

int ModelRotation::normalizeAngle(int angle) { return ((angle % 360) + 360) % 360; }

static constexpr std::pair<int, int> k_rotationDefs[] = {
    {0, 0},   {0, 90},   {0, 180},   {0, 270},   {90, 0},  {90, 90},  {90, 180},  {90, 270},
    {180, 0}, {180, 90}, {180, 180}, {180, 270}, {270, 0}, {270, 90}, {270, 180}, {270, 270},
};

static constexpr const char* k_rotationNames[] = {
    "X0_Y0",   "X0_Y90",   "X0_Y180",   "X0_Y270",   "X90_Y0",  "X90_Y90",  "X90_Y180",  "X90_Y270",
    "X180_Y0", "X180_Y90", "X180_Y180", "X180_Y270", "X270_Y0", "X270_Y90", "X270_Y180", "X270_Y270",
};

const std::array<ModelRotation, static_cast<std::size_t>(ModelRotation::Value::COUNT)>& ModelRotation::allValues() {
    static auto s_values = []() {
        std::array<ModelRotation, static_cast<std::size_t>(Value::COUNT)> arr = {
            ModelRotation(Value::X0_Y0),     ModelRotation(Value::X0_Y90),    ModelRotation(Value::X0_Y180),
            ModelRotation(Value::X0_Y270),   ModelRotation(Value::X90_Y0),    ModelRotation(Value::X90_Y90),
            ModelRotation(Value::X90_Y180),  ModelRotation(Value::X90_Y270),  ModelRotation(Value::X180_Y0),
            ModelRotation(Value::X180_Y90),  ModelRotation(Value::X180_Y180), ModelRotation(Value::X180_Y270),
            ModelRotation(Value::X270_Y0),   ModelRotation(Value::X270_Y90),  ModelRotation(Value::X270_Y180),
            ModelRotation(Value::X270_Y270),
        };
        return arr;
    }();
    return s_values;
}

const std::unordered_map<int, ModelRotation::Value>& ModelRotation::rotationMap() {
    static auto s_map = []() {
        std::unordered_map<int, Value> map;
        for (const auto& rot : allValues())
            map.emplace(combineXY(rot.m_x, rot.m_y), rot.m_value);
        return map;
    }();
    return s_map;
}

ModelRotation::ModelRotation(Value value) :
    m_value(value), m_x(k_rotationDefs[static_cast<int>(value)].first),
    m_y(k_rotationDefs[static_cast<int>(value)].second), m_quartersX(std::abs(m_x / 90)),
    m_quartersY(std::abs(m_y / 90)), m_matrix(Amy::mat4::identity()) {
    Amy::mat4 rotX = Amy::mat4::rotateX(Amy::Radians(static_cast<float>(-m_x)));
    Amy::mat4 rotY = Amy::mat4::rotateY(Amy::Radians(static_cast<float>(-m_y)));
    m_matrix = rotY * rotX;
}

std::optional<ModelRotation> ModelRotation::getModelRotation(int x, int y) {
    int key = combineXY(normalizeAngle(x), normalizeAngle(y));
    auto it = rotationMap().find(key);
    if (it == rotationMap().end())
        return std::nullopt;
    return allValues()[static_cast<std::size_t>(it->second)];
}

const Amy::mat4& ModelRotation::getMatrix4d() const { return m_matrix; }
int ModelRotation::getX() const { return m_x; }
int ModelRotation::getY() const { return m_y; }
ModelRotation::Value ModelRotation::getValue() const { return m_value; }

EnumFacing ModelRotation::rotateFace(EnumFacing facing) const {
    EnumFacing result = facing;

    for (int i = 0; i < m_quartersX; ++i)
        result = result.rotateAround(EnumFacing::Axis::X);

    if (result.getAxis() != EnumFacing::Axis::Y) {
        for (int j = 0; j < m_quartersY; ++j)
            result = result.rotateAround(EnumFacing::Axis::Y);
    }

    return result;
}

int ModelRotation::rotateVertex(EnumFacing facing, int vertexIndex) const {
    int i = vertexIndex;

    if (facing.getAxis() == EnumFacing::Axis::X)
        i = (vertexIndex + m_quartersX) % 4;

    EnumFacing rotated = facing;
    for (int j = 0; j < m_quartersX; ++j)
        rotated = rotated.rotateAround(EnumFacing::Axis::X);

    if (rotated.getAxis() == EnumFacing::Axis::Y)
        i = (i + m_quartersY) % 4;

    return i;
}

bool ModelRotation::operator==(const ModelRotation& other) const { return m_value == other.m_value; }

bool ModelRotation::operator!=(const ModelRotation& other) const { return !(*this == other); }

std::size_t ModelRotation::hashCode() const { return static_cast<std::size_t>(m_value); }

std::string ModelRotation::toString() const { return k_rotationNames[static_cast<std::size_t>(m_value)]; }
