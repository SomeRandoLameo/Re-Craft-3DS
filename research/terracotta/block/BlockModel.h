#ifndef TERRACOTTA_BLOCK_BLOCKMODEL_H_
#define TERRACOTTA_BLOCK_BLOCKMODEL_H_

#include <string>
#include <memory>
#include <unordered_map>

#include <mclib/common/JsonFwd.h>
#include "BlockFace.h"
#include "BlockElement.h"

namespace terra {
namespace block {

class BlockModel {
public:
    std::vector<BlockElement>& GetElements() { return m_Elements; }
    void AddElement(BlockElement element) { m_Elements.push_back(element); }

private:
    std::vector<BlockElement> m_Elements;
};

} // ns block
} // ns terra

#endif
