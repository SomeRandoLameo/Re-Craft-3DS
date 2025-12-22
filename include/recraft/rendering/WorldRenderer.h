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
    Chunk* cluster;
    ChunkColumn* chunk;
    Direction enteredFrom;
};

struct TransparentRender {
    Chunk* cluster;
    ChunkColumn* chunk;
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

    Hand* m_hand;
    CubeRenderer* m_cubeRenderer;
    Clouds* m_clouds;
    Player* m_player;
    World* m_world;
    WorkQueue* m_workqueue;
    Camera m_cam;
    int m_projectionUniform;

    Cursor* m_cursor;

    std::vector<RenderStep> m_renderingQueue;
    uint8_t m_chunkRendered[CHUNKCACHE_SIZE][CLUSTER_PER_CHUNK][CHUNKCACHE_SIZE];
    std::vector<TransparentRender> m_transparentClusters;

    C3D_FogLut m_fogLut;

    inline bool ClusterWasRendered(int x, int y, int z) const {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (CHUNKCACHE_SIZE / 2))][y]
        [z - (m_world->cacheTranslationZ - (CHUNKCACHE_SIZE / 2))];
    }

    inline uint8_t& ClusterRenderedRef(int x, int y, int z) {
        return m_chunkRendered[x - (m_world->cacheTranslationX - (CHUNKCACHE_SIZE / 2))][y]
        [z - (m_world->cacheTranslationZ - (CHUNKCACHE_SIZE / 2))];
    }
};