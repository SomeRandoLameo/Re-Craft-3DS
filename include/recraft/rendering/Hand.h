#pragma once

#include <citro3d.h>

#include "../entity/Player.h"
#include "../blocks/CT_Block.h"
#include "misc/NumberUtils.h"
#include "rendering/VertexFmt.h"

#include "rendering/TextureMap.h"

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
