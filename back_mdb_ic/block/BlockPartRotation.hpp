#pragma once

#include "Vector.h"
#include "util/EnumFacing.hpp"

struct BlockPartRotation {
    mc::Vector3f origin;
    EnumFacing::Axis::Value axis;
    float angle;
    bool rescale;

    BlockPartRotation(mc::Vector3f originIn, EnumFacing::Axis::Value axisIn, float angleIn, bool rescaleIn) :
        origin(originIn), axis(axisIn), angle(angleIn), rescale(rescaleIn) {}
};
