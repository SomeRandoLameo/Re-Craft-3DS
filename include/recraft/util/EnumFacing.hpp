#pragma once

#include <optional>
#include <stdexcept>
#include <string>

class EnumFacing {
public:
    enum class Value { DOWN, UP, NORTH, SOUTH, WEST, EAST };

    constexpr static Value DOWN = Value::DOWN;
    constexpr static Value UP = Value::UP;
    constexpr static Value NORTH = Value::NORTH;
    constexpr static Value SOUTH = Value::SOUTH;
    constexpr static Value WEST = Value::WEST;
    constexpr static Value EAST = Value::EAST;

    constexpr EnumFacing(Value v) : m_value(v) {}

    friend bool operator<(Value a, Value b) { return static_cast<int>(a) < static_cast<int>(b); }
    bool operator==(const EnumFacing& o) const { return m_value == o.m_value; }
    bool operator!=(const EnumFacing& o) const { return m_value != o.m_value; }

    static std::optional<Value> byName(const std::string& name) {
        if (name == "down")
            return DOWN;
        if (name == "up")
            return UP;
        if (name == "north")
            return NORTH;
        if (name == "south")
            return SOUTH;
        if (name == "west")
            return WEST;
        if (name == "east")
            return EAST;
        return std::nullopt;
    }

    class Axis {
    public:
        enum class Value { X, Y, Z };

        constexpr static Value X = Value::X;
        constexpr static Value Y = Value::Y;
        constexpr static Value Z = Value::Z;

        static std::optional<Value> byName(const std::string& name) {
            if (name == "x")
                return X;
            if (name == "y")
                return Y;
            if (name == "z")
                return Z;
            return std::nullopt;
        }
    };

    Axis::Value getAxis() const {
        switch (m_value) {
        case Value::WEST:
        case Value::EAST:
            return Axis::X;
        case Value::DOWN:
        case Value::UP:
            return Axis::Y;
        case Value::NORTH:
        case Value::SOUTH:
            return Axis::Z;
        }
    }

    EnumFacing rotateAround(Axis::Value axis) const {
        switch (axis) {
        case Axis::X:
            if (m_value != WEST && m_value != EAST)
                return rotateX();
            return *this;
        case Axis::Y:
            if (m_value != UP && m_value != DOWN)
                return rotateY();
            return *this;
        case Axis::Z:
            if (m_value != NORTH && m_value != SOUTH)
                return rotateZ();
            return *this;
        }
    }

private:
    Value m_value;

    EnumFacing rotateX() const {
        switch (m_value) {
        case Value::NORTH:
            return DOWN;
        case Value::DOWN:
            return SOUTH;
        case Value::SOUTH:
            return UP;
        case Value::UP:
            return NORTH;
        default:
            throw std::runtime_error("Unable to get X-rotated facing");
        }
    }

    EnumFacing rotateY() const {
        switch (m_value) {
        case Value::NORTH:
            return EAST;
        case Value::EAST:
            return SOUTH;
        case Value::SOUTH:
            return WEST;
        case Value::WEST:
            return NORTH;
        default:
            throw std::runtime_error("Unable to get Y-rotated facing");
        }
    }

    EnumFacing rotateZ() const {
        switch (m_value) {
        case Value::EAST:
            return DOWN;
        case Value::WEST:
            return UP;
        case Value::UP:
            return EAST;
        case Value::DOWN:
            return WEST;
        default:
            throw std::runtime_error("Unable to get Z-rotated facing");
        }
    }
};
