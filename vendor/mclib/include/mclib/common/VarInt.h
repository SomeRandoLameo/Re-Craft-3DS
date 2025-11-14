#pragma once

#include "../mclib.h"
#include "Types.h"

#include <iosfwd>

namespace mc {

    class DataBuffer;

    class VarInt {
    private:
        s64 m_Value;

    public:
        VarInt() noexcept;
        VarInt(s8 val) noexcept;
        VarInt(s16 val) noexcept;
        VarInt(s32 val) noexcept;
        VarInt(s64 val) noexcept;

        s8 GetByte() const noexcept { return (s8)m_Value; }
        s16 GetShort() const noexcept { return (s16)m_Value; }
        s32 GetInt() const noexcept { return (s32)m_Value; }
        s64 GetLong() const noexcept { return m_Value; }

        // Returns how many bytes this will take up in a buffer
        std::size_t GetSerializedLength() const;

        friend DataBuffer& operator<<(DataBuffer& out, const VarInt& pos);
        friend DataBuffer& operator>>(DataBuffer& in, VarInt& pos);
    };

    typedef VarInt VarLong;

    DataBuffer& operator<<(DataBuffer& out, const VarInt& var);
    DataBuffer& operator>>(DataBuffer& in, VarInt& var);

} // ns mc

std::ostream& operator<<(std::ostream& out, const mc::VarInt& v);


