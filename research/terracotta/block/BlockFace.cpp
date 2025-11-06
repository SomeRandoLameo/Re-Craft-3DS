#include "BlockFace.h"
#include <algorithm>
#include <cctype>

namespace terra {
namespace block {

const std::string face_strings[] = {
    "North", "East", "South", "West", "Up", "Down"
};

const std::string& to_string(BlockFace face) {
    return face_strings[static_cast<unsigned int>(face)];
}

BlockFace face_from_string(const std::string& str) {
    std::string find;

    find.resize(str.size());

    std::transform(str.begin(), str.end(), find.begin(), tolower);

    find[0] = toupper(find[0]);
    
    for (std::size_t i = 0; i < 6; ++i) {
        if (face_strings[i] == find) {
            return static_cast<BlockFace>(i);
        }
    }

    return BlockFace::None;
}

BlockFace get_opposite_face(BlockFace face) {
    if (face == BlockFace::East) {
        return BlockFace::West;
    } else if (face == BlockFace::West) {
        return BlockFace::East;
    } else if (face == BlockFace::North) {
        return BlockFace::South;
    } else if (face == BlockFace::South) {
        return BlockFace::North;
    } else if (face == BlockFace::Up) {
        return BlockFace::Down;
    } else if (face == BlockFace::Down) {
        return BlockFace::Up;
    }

    return BlockFace::None;
}

} // ns block
} // ns terra
