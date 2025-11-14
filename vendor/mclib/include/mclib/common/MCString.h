#pragma once

#include "../mclib.h"
#include "Types.h"

#include <string>

namespace mc {

    class DataBuffer;

    class MCString {
    private:
        std::wstring m_UTF16;

    public:
        MCString();
        MCString(const std::string& str);
        MCString(const std::wstring& str);

        std::wstring GetUTF16() const;
        std::string GetUTF8() const;


        static MCString FromUTF8(const std::string& utf8);

        friend DataBuffer& operator<<(DataBuffer& out, const MCString& str);
        friend DataBuffer& operator>>(DataBuffer& in, MCString& str);
    };

    std::string utf16to8(std::wstring str);
    std::wstring utf8to16(std::string str);

    DataBuffer& operator<<(DataBuffer& out, const MCString& pos);
    DataBuffer& operator>>(DataBuffer& in, MCString& pos);

} // ns mc