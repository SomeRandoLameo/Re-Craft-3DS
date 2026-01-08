#pragma once

#include "../world/CT_World.h"
#include "../entity/Player.h"
#include "../world/WorkQueue.h"
#include "../mcbridge/MCBridge.h"
#include "Camera.h"
#include "Cursor.h"
#include "Hand.h"
#include "Clouds.h"
#include "CubeRenderer.h"
#include <citro3d.h>

//mclib
#include <mclib/common/Vector.h>

struct RenderStep {
    ChunkPtr chunk;
    ChunkColumnPtr column;
    Direction enteredFrom;
};

struct TransparentRender {
    ChunkPtr chunk;
    ChunkColumnPtr column;
};

class WorldRenderer {
public:
    WorldRenderer(Player* player, World* world, WorkQueue* workqueue, int projectionUniform);
    ~WorldRenderer();

    WorldRenderer(const WorldRenderer&) = delete;
    WorldRenderer& operator=(const WorldRenderer&) = delete;

    void Render(float iod);

private:
    void RenderWorld();

    Hand* m_hand = nullptr;
    CubeRenderer* m_cubeRenderer = nullptr;
    Clouds* m_clouds = nullptr;
    Player* m_player = nullptr;
    World* m_world = nullptr;
    WorkQueue* m_workqueue = nullptr;
    Cursor* m_cursor = nullptr;
    Camera m_cam;

    std::vector<RenderStep> m_renderingQueue;

    uint8_t m_chunkRendered[World::ChunkCacheSize][ChunkColumn::ChunksPerColumn][World::ChunkCacheSize];
    std::vector<TransparentRender> m_transparentClusters;
    C3D_FogLut m_fogLut;

    int m_projectionUniform;

    inline bool ClusterWasRendered(int x, int y, int z) const {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (World::ChunkCacheSize / 2))][y]
        [z - (m_world->cacheTranslationZ - (World::ChunkCacheSize / 2))];
    }

    inline uint8_t& ClusterRenderedRef(int x, int y, int z) {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (World::ChunkCacheSize / 2))][y]
        [z - (m_world->cacheTranslationZ - (World::ChunkCacheSize / 2))];
    }
};