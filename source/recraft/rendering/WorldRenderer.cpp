#include "blocks/CT_Block.h"
#include "gui/DebugUI.h"
#include "rendering/Clouds.h"
#include "rendering/Hand.h"
#include "rendering/VertexFmt.h"
#include "rendering/WorldRenderer.h"


#include <citro3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ranges>

int sky_time = 0;

WorldRenderer::WorldRenderer(Player* player, World* world, WorkQueue* workqueue, int projectionUniform) :
    m_player(player), m_world(world), m_workqueue(workqueue), m_projectionUniform(projectionUniform),
    m_cursor(nullptr) {

    m_renderingQueue.clear();
    m_transparentClusters.clear();

    m_cursor = new Cursor();

    m_hand = new Hand();

    // what the hell is event that.
    float data[256];
    for (int i = 0; i <= 128; i++) {
        float val = 1.f;
        if (i == 0)
            val = 0.0f;
        else if (i == 1)
            val = 0.9f;
        if (i < 128)
            data[i] = val;
        if (i > 0)
            data[i + 127] = val - data[i - 1];
    }
    FogLut_FromArray(&m_fogLut, data);
    C3D_FogGasMode(GPU_FOG, GPU_PLAIN_DENSITY, false);
    C3D_FogColor(0xffd990);
    C3D_FogLutBind(&m_fogLut);

    m_clouds = new Clouds();
}

WorldRenderer::~WorldRenderer() {
    m_renderingQueue.clear();
    m_transparentClusters.clear();

    delete m_cursor;
    m_cursor = nullptr;

    m_hand->~Hand();
    m_clouds->~Clouds();
}

void WorldRenderer::RenderWorld() {
    C3D_FogColor(0xffd990);

    memset(m_chunkRendered, 0, sizeof(m_chunkRendered));

    int polysTotal = 0, clustersDrawn = 0, steps = 0;

    m_renderingQueue.clear();
    m_transparentClusters.clear();

    int pY = CLAMP(WorldToChunkCoord(FastFloor(m_player->position.y)), 0, CLUSTER_PER_CHUNK - 1);
    ChunkColumn* pChunk = m_world->GetChunkColumn(WorldToChunkCoord(FastFloor(m_player->position.x)),
                                                  WorldToChunkCoord(FastFloor(m_player->position.z)));
    m_renderingQueue.push_back(RenderStep{&pChunk->chunks[pY], pChunk, Direction_Invalid});
    ClusterRenderedRef(CHUNKCACHE_SIZE / 2 + m_world->cacheTranslationX, pY,
                       CHUNKCACHE_SIZE / 2 + m_world->cacheTranslationZ) = 1;

    mc::Vector3d playerPos = m_player->position;

    while (!m_renderingQueue.empty()) {
        RenderStep step = m_renderingQueue.back();
        m_renderingQueue.pop_back();
        ChunkColumn* chunk = step.chunk;
        Chunk* cluster = step.cluster;

        if (cluster->vertices > 0 && cluster->vbo.size) {
            ClusterRenderedRef(chunk->x, cluster->y, chunk->z) |= 2;

            C3D::BufCfg<4>(cluster->vbo.memory, sizeof(WorldVertex));
            C3D::DrawArrays(0, cluster->vertices);

            polysTotal += cluster->vertices;
            clustersDrawn++;
        }
        if (cluster->transparentVertices > 0 && cluster->transparentVBO.size) {
            m_transparentClusters.push_back(TransparentRender{cluster, chunk});
        }

        for (int i = 0; i < 6; i++) {
            Direction dir = (Direction)i;
            const int* offset = DirectionToOffset[dir];

            int newX = chunk->x + offset[0], newY = cluster->y + offset[1], newZ = chunk->z + offset[2];
            if (newX < m_world->cacheTranslationX - CHUNKCACHE_SIZE / 2 + 1 ||
                newX > m_world->cacheTranslationX + CHUNKCACHE_SIZE / 2 - 1 ||
                newZ < m_world->cacheTranslationZ - CHUNKCACHE_SIZE / 2 + 1 ||
                newZ > m_world->cacheTranslationZ + CHUNKCACHE_SIZE / 2 - 1 || newY < 0 || newY >= CLUSTER_PER_CHUNK)
                continue;

            mc::Vector3d dist = mc::Vector3d(newX * CHUNK_SIZE + CHUNK_SIZE / 2, newY * CHUNK_SIZE + CHUNK_SIZE / 2,
                                             newZ * CHUNK_SIZE + CHUNK_SIZE / 2) -
                playerPos;
            if (Vector3d_dot(dist, dist) > (3.f * CHUNK_SIZE) * (3.f * CHUNK_SIZE)) {
                continue;
            }

            if (ClusterRenderedRef(newX, newY, newZ) & 1)
                continue;

            if (!ChunkCanBeSeenThrough(cluster->seeThrough, step.enteredFrom, (Direction)i) &&
                step.enteredFrom != Direction_Invalid)
                continue;

            C3D_FVec chunkPosition = FVec3_New(newX * CHUNK_SIZE, newY * CHUNK_SIZE, newZ * CHUNK_SIZE);
            if (!m_cam.IsAABBVisible(chunkPosition, FVec3_New(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)))
                continue;

            ClusterRenderedRef(newX, newY, newZ) |= 1;

            ChunkColumn* newChunk = m_world->GetChunkColumn(newX, newZ);
            RenderStep nextStep = (RenderStep){&newChunk->chunks[newY], newChunk, DirectionOpposite[dir]};
            if (newChunk)
                m_renderingQueue.push_back(nextStep);
        }
    }

    for (int x = 1; x < CHUNKCACHE_SIZE - 1; x++) {
        for (int z = 1; z < CHUNKCACHE_SIZE - 1; z++) {
            ChunkColumn* chunk = m_world->chunkCache[x][z];

            if ((chunk->revision != chunk->displayRevision || chunk->forceVBOUpdate) && !chunk->tasksRunning) {
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++)
                    for (int zOff = -1; zOff < 2 && clear; zOff++)
                        if (m_world->chunkCache[x + xOff][z + zOff]->genProgress == ChunkGen_Empty)
                            clear = false;

                if (clear)
                    WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_PolyGen, chunk});
            }
        }
    }

    C3D_AlphaTest(true, GPU_GEQUAL, 255);

    for (auto& m_transparentCluster : std::ranges::reverse_view(m_transparentClusters)) {
        TransparentRender* render = &m_transparentCluster;

        C3D::BufCfg<4>(render->cluster->transparentVBO.memory, sizeof(WorldVertex));
        C3D::DrawArrays(0, render->cluster->transparentVertices);

        polysTotal += render->cluster->transparentVertices;
    }
    C3D_AlphaTest(false, GPU_GREATER, 0);

    // DebugUI_Text("Clusters drawn %d with %d steps. %d vertices", clustersDrawn, steps, polysTotal);
    // DebugUI_Text("T: %u P: %u %d", world->chunkCache[CHUNKCACHE_SIZE / 2][CHUNKCACHE_SIZE / 2]->tasksRunning,
    //			 world->chunkCache[CHUNKCACHE_SIZE / 2][CHUNKCACHE_SIZE / 2]->genProgress,
    //			 workqueue->queue.length);
}

void WorldRenderer::Render(float iod) {
    m_cam.Update(m_player, iod);

    m_hand->Draw(m_projectionUniform, m_cam.GetProjection(), m_player->quickSelectBar[m_player->quickSelectBarSlot],
                 m_player);

    C3D_TexBind(0, (C3D_Tex*)Block_GetTextureMap());

    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, m_cam.GetVP());

    RenderWorld();

    m_clouds->Draw(m_projectionUniform, m_cam.GetVP(), m_world, m_player->position.x, m_player->position.z);

    if (m_player->blockInActionRange) {
        m_cursor->Draw(m_projectionUniform, m_cam.GetVP(), m_world, m_player->viewRayCast.x, m_player->viewRayCast.y,
                       m_player->viewRayCast.z, m_player->viewRayCast.direction);
    }
}
