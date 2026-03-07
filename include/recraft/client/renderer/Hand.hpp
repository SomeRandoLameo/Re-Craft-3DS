#pragma once

#include <citro3d.h>
#include <string.h>

#include <amethyst.hpp>

#include "TextureMap.hpp"
#include "VertexFmt.hpp"
#include "block/Block.hpp"
#include "client/entity/Player.hpp"
#include "misc/NumberUtils.hpp"

class Hand {
public:
    Hand();
    ~Hand();
    void Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player);

private:
    WorldVertex* m_handVBO = nullptr;
    Amy::Texture m_SkinTexture;
};
