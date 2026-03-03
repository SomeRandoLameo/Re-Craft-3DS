#pragma once

#include "../../../../../../../../opt/devkitpro/libctru/include/citro3d.h"

#include "../../blocks/Block.hpp"
#include "../../misc/NumberUtils.hpp"
#include "VertexFmt.hpp"
#include "client/entity/Player.hpp"

#include "TextureMap.hpp"

#include "../../../../../../../../opt/devkitpro/devkitARM/arm-none-eabi/include/string.h"

class Hand {
public:
    Hand();
    ~Hand();
    void Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player);

private:
    WorldVertex* m_handVBO = nullptr;
    C3D_Tex m_SkinTexture;
};
