#pragma once

enum Direction { West, East, Bottom, Top, North, South, Invalid };

enum Axis { X, Y, Z };

extern int       DirectionToOffset[7][3];
extern Direction DirectionOpposite[7];
extern Axis      DirectionToAxis[7];
