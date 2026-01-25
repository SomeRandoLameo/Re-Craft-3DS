#include "world/Direction.hpp"

int       DirectionToOffset[7][3] = {{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}, {0, 0, 0}};
Direction DirectionOpposite[7]    = {Direction::East,  Direction::West,  Direction::Top,    Direction::Bottom,
                                     Direction::South, Direction::North, Direction::Invalid};
Axis      DirectionToAxis[7]      = {Axis::X, Axis::X, Axis::Y, Axis::Y, Axis::Z, Axis::Z, Axis::X};
