#pragma once

#include "../mclib.h"
#include "Types.h"
#include <iosfwd>
#include <string>

namespace mc {

    class DataBuffer;

    class Position {
    private:
        s64 m_X;
        s64 m_Y;
        s64 m_Z;

    public:
        Position() noexcept : m_X(0), m_Y(0), m_Z(0) { }
        Position(s32 x, s32 y, s32 z) noexcept;
        s64 Encode64() const noexcept;

        s64 GetX() const { return m_X; }
        s64 GetY() const { return m_Y; }
        s64 GetZ() const { return m_Z; }

        friend DataBuffer& operator<<(DataBuffer& out, const Position& pos);
        friend DataBuffer& operator>>(DataBuffer& in, Position& pos);
    };

    DataBuffer& operator<<(DataBuffer& out, const Position& pos);
    DataBuffer& operator>>(DataBuffer& in, Position& pos);

    std::string to_string(const Position& data);

} // ns mc

std::ostream& operator<<(std::ostream& out, const mc::Position& pos);
std::wostream& operator<<(std::wostream& out, const mc::Position& pos);

