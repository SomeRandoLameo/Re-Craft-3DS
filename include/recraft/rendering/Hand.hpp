#pragma once

#include <citro3d.h>

#include "blocks/CT_Block.hpp"

class Player;
class WorldVertex;
class Hand {
public:
    Hand();
    ~Hand();
    void Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player);

private:
    WorldVertex* m_handVBO = nullptr;
    C3D_Tex      m_SkinTexture;
};
