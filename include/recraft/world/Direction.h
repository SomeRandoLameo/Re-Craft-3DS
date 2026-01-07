#pragma once

typedef enum {
    West,
    East,
    Bottom,
    Top,
    North,
    South,
    Invalid
} Direction;

typedef enum {
    X,
    Y,
    Z
} Axis;

extern int DirectionToOffset[7][3];
extern Direction DirectionOpposite[7];
extern Axis DirectionToAxis[7];