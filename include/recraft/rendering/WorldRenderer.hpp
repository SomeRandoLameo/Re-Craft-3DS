#pragma once

#include <citro3d.h>
#include "../entity/Player.hpp"
#include "../world/CT_World.hpp"
#include "../world/WorkQueue.hpp"
#include "Camera.hpp"
#include "Clouds.hpp"
#include "CubeRenderer.hpp"
#include "Cursor.hpp"
#include "Hand.hpp"
#include "mcbridge/MCBridge.hpp"

// mclib
#include <mclib/common/Vector.h>

struct RenderStep {
    ChunkPtr       chunk;
    ChunkColumnPtr column;
    Direction      enteredFrom;
};

struct TransparentRender {
    ChunkPtr       chunk;
    ChunkColumnPtr column;
};

class WorldRenderer {
public:
    WorldRenderer(Player* player, World* world, WorkQueue* workqueue, int projectionUniform);
    ~WorldRenderer();

    WorldRenderer(const WorldRenderer&)            = delete;
    WorldRenderer& operator=(const WorldRenderer&) = delete;

    void Render(float iod);

private:
    void RenderWorld();

    Hand*         m_hand         = nullptr;
    CubeRenderer* m_cubeRenderer = nullptr;
    Clouds*       m_clouds       = nullptr;
    Player*       m_player       = nullptr;
    World*        m_world        = nullptr;
    WorkQueue*    m_workqueue    = nullptr;
    Cursor*       m_cursor       = nullptr;
    Camera        m_cam;

    std::vector<RenderStep> m_renderingQueue;

    u8 m_chunkRendered[World::ChunkCacheSize][ChunkColumn::ChunksPerColumn][World::ChunkCacheSize];
    std::vector<TransparentRender> m_transparentClusters;
    C3D_FogLut                     m_fogLut;

    int m_projectionUniform;

    inline bool ClusterWasRendered(int x, int y, int z) const {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (World::ChunkCacheSize / 2))][y]
                              [z - (m_world->cacheTranslationZ - (World::ChunkCacheSize / 2))];
    }

    inline u8& ClusterRenderedRef(int x, int y, int z) {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (World::ChunkCacheSize / 2))][y]
                              [z - (m_world->cacheTranslationZ - (World::ChunkCacheSize / 2))];
    }
};
