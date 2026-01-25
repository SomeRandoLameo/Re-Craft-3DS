#include "rendering/WorldRenderer.hpp"
#include "blocks/CT_Block.hpp"
#include "gui/DebugUI.hpp"
#include "rendering/Clouds.hpp"
#include "rendering/Hand.hpp"
#include "rendering/VertexFmt.hpp"


#include <citro3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ranges>

int sky_time = 0;

WorldRenderer::WorldRenderer(Player* player, World* world, WorkQueue* workqueue, int projectionUniform) :
    m_player(player), m_world(world), m_workqueue(workqueue), m_projectionUniform(projectionUniform) {

    m_renderingQueue.clear();
    m_transparentClusters.clear();

    m_cursor = new Cursor();

    m_hand = new Hand();

    m_cubeRenderer = new CubeRenderer();

    // what the hell is event that.
    float data[256];
    for (int i = 0; i <= 128; i++) {
        float val = 1.f;
        if (i == 0) {
            val = 0.0f;
        } else if (i == 1) {
            val = 0.9f;
        }
        if (i < 128) {
            data[i] = val;
        }
        if (i > 0) {
            data[i + 127] = val - data[i - 1];
        }
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
    m_cubeRenderer->~CubeRenderer();
    m_clouds->~Clouds();
}

void WorldRenderer::RenderWorld() {
    C3D_FogColor(0xffd990);

    memset(m_chunkRendered, 0, sizeof(m_chunkRendered));

    int polysTotal = 0, clustersDrawn = 0, steps = 0;

    m_renderingQueue.clear();
    m_transparentClusters.clear();

    int pY = CLAMP(WorldToChunkCoord(FastFloor(m_player->position.y)), 0, ChunkColumn::ChunksPerColumn - 1);

    ChunkColumnPtr columnPtr = m_world->GetChunkColumn(WorldToChunkCoord(FastFloor(m_player->position.x)),
                                                       WorldToChunkCoord(FastFloor(m_player->position.z)));

    if (!columnPtr) {
        // Player is in an invalid/unloaded chunk!
        return;
    }


    m_renderingQueue.push_back(RenderStep{columnPtr->GetChunk(pY), columnPtr, Direction::Invalid});

    ClusterRenderedRef(World::ChunkCacheSize / 2 + m_world->cacheTranslationX, pY,
                       World::ChunkCacheSize / 2 + m_world->cacheTranslationZ) = 1;

    mc::Vector3f playerPos = m_player->position;

    while (!m_renderingQueue.empty()) {
        RenderStep step = m_renderingQueue.back();
        m_renderingQueue.pop_back();
        ChunkColumnPtr column = step.column;
        ChunkPtr       chunk  = step.chunk;

        if (chunk->vertices > 0 && chunk->vbo.size) {
            ClusterRenderedRef(column->x, chunk->y, column->z) |= 2;

            C3D::BufCfg<4>(chunk->vbo.memory, sizeof(WorldVertex));
            C3D::DrawArrays(0, chunk->vertices);

            polysTotal += chunk->vertices;
            clustersDrawn++;
        }
        if (chunk->transparentVertices > 0 && chunk->transparentVBO.size) {
            m_transparentClusters.push_back(TransparentRender{chunk, column});
        }

        for (int i = 0; i < 6; i++) {
            Direction  dir    = (Direction)i;
            const int* offset = DirectionToOffset[dir];

            int newX = column->x + offset[0], newY = chunk->y + offset[1], newZ = column->z + offset[2];
            if (newX < m_world->cacheTranslationX - World::ChunkCacheSize / 2 + 1 ||
                newX > m_world->cacheTranslationX + World::ChunkCacheSize / 2 - 1 ||
                newZ < m_world->cacheTranslationZ - World::ChunkCacheSize / 2 + 1 ||
                newZ > m_world->cacheTranslationZ + World::ChunkCacheSize / 2 - 1 || newY < 0 ||
                newY >= ChunkColumn::ChunksPerColumn) {
                continue;
            }

            mc::Vector3f dist = mc::Vector3f(newX * Chunk::Size + Chunk::Size / 2, newY * Chunk::Size + Chunk::Size / 2,
                                             newZ * Chunk::Size + Chunk::Size / 2) -
                                playerPos;
            if (Vector3f_dot(dist, dist) > (3.f * Chunk::Size) * (3.f * Chunk::Size)) {
                continue;
            }

            if (ClusterRenderedRef(newX, newY, newZ) & 1) {
                continue;
            }

            if (!ChunkCanBeSeenThrough(chunk->seeThrough, step.enteredFrom, (Direction)i) &&
                step.enteredFrom != Direction::Invalid) {
                continue;
            }

            C3D_FVec chunkPosition = FVec3_New(newX * Chunk::Size, newY * Chunk::Size, newZ * Chunk::Size);
            if (!m_cam.IsAABBVisible(chunkPosition, FVec3_New(Chunk::Size, Chunk::Size, Chunk::Size))) {
                continue;
            }

            ClusterRenderedRef(newX, newY, newZ) |= 1;

            ChunkColumnPtr newColumn = m_world->GetChunkColumn(newX, newZ);
            if (!newColumn) {
                continue; // Column doesn't exist, skip this neighbor
            }

            ChunkPtr   nextChunk = newColumn->GetChunk(newY);
            RenderStep nextStep  = (RenderStep){nextChunk, newColumn, DirectionOpposite[dir]};
            m_renderingQueue.push_back(nextStep);
        }
    }

    for (int x = 1; x < World::ChunkCacheSize - 1; x++) {
        for (int z = 1; z < World::ChunkCacheSize - 1; z++) {
            ChunkColumnPtr column = m_world->columnCache[x][z];

            if ((column->revision != column->displayRevision || column->forceVBOUpdate) && !column->tasksRunning) {
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++) {
                    for (int zOff = -1; zOff < 2 && clear; zOff++) {
                        if (m_world->columnCache[x + xOff][z + zOff]->genProgress == ChunkGen_Empty) {
                            clear = false;
                        }
                    }
                }

                if (clear) {
                    WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType::PolyGen, column});
                }
            }
        }
    }

    C3D_AlphaTest(true, GPU_GEQUAL, 255);

    for (auto& m_transparentCluster : std::ranges::reverse_view(m_transparentClusters)) {
        TransparentRender* render = &m_transparentCluster;

        C3D::BufCfg<4>(render->chunk->transparentVBO.memory, sizeof(WorldVertex));
        C3D::DrawArrays(0, render->chunk->transparentVertices);

        polysTotal += render->chunk->transparentVertices;
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

    m_clouds->Draw(m_projectionUniform, m_cam.GetVP(), m_player->position.x, m_player->position.z);

    m_cubeRenderer->Draw(m_projectionUniform, m_cam.GetVP(), m_world, m_player->position);

    if (m_player->blockInActionRange) {
        m_cursor->Draw(m_projectionUniform, m_cam.GetVP(), m_world, m_player->viewRayCast.hitPos,
                       m_player->viewRayCast.direction);
    }
}
