#ifndef TERRACOTTA_BLOCK_BLOCKSTATE_H_
#define TERRACOTTA_BLOCK_BLOCKSTATE_H_

#include <mclib/common/Types.h>
#include <mclib/common/JsonFwd.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace terra {
namespace block {

class BlockState {
public:
    BlockState(u32 id) : m_Id(id) { }

    u32 GetId() const { return m_Id; }
    const std::string& GetProperty(const std::string& property) const;
    const std::string& GetVariant() const { return m_Variant; }

    static std::unique_ptr<BlockState> FromJSON(const mc::json& json);

private:
    u32 m_Id;
    std::string m_Variant;
    std::unordered_map<std::string, std::string> m_Properties;
};

} // ns block
} // ns terra

#endif
