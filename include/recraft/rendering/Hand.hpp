#pragma once

#include <citro3d.h>

#include "../blocks/Block.hpp"
#include "../entity/Player.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/VertexFmt.hpp"

#include "rendering/TextureMap.hpp"

#include <string.h>

class Hand {
public:
    Hand();
    ~Hand();
    void Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player);
private:
    WorldVertex* m_handVBO = nullptr;
    C3D_Tex m_SkinTexture;
};
