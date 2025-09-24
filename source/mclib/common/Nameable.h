#pragma once

#include <string>

class Nameable {
private:
    std::wstring m_CustomName;

public:
    virtual ~Nameable() { }

    const std::wstring& GetCustomName() const noexcept { return m_CustomName; }
    void SetCustomName(const std::wstring& name) { m_CustomName = name; }
};