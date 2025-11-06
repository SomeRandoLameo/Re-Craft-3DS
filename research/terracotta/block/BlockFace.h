#ifndef TERRACOTTA_BLOCK_BLOCKFACE_H_
#define TERRACOTTA_BLOCK_BLOCKFACE_H_

#include <string>

namespace terra {
namespace block {

enum class BlockFace {
    North,
    East,
    South,
    West,
    Up,
    Down,

    None
};

const std::string& to_string(BlockFace face);
BlockFace face_from_string(const std::string& str);
BlockFace get_opposite_face(BlockFace face);

} // ns block
} // ns terra

#endif
