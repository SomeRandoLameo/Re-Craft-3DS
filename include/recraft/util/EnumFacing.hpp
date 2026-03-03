#pragma once

#include <optional>
#include <string>
// TODO: Add more + separate
class EnumFacing {
public:
    enum class Value { DOWN, UP, NORTH, SOUTH, WEST, EAST };

    constexpr static Value DOWN = Value::DOWN;
    constexpr static Value UP = Value::UP;
    constexpr static Value NORTH = Value::NORTH;
    constexpr static Value SOUTH = Value::SOUTH;
    constexpr static Value WEST = Value::WEST;
    constexpr static Value EAST = Value::EAST;

    friend bool operator<(Value a, Value b) { return static_cast<int>(a) < static_cast<int>(b); }

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
};
