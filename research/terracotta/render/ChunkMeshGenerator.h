#ifndef TERRACOTTA_RENDER_CHUNKMESHGENERATOR_H_
#define TERRACOTTA_RENDER_CHUNKMESHGENERATOR_H_

#include "ChunkMesh.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <utility>
#include <mclib/common/Vector.h>
#include <glm/glm.hpp>
#include "../World.h"
#include "../PriorityQueue.h"
#include "../block/BlockFace.h"
#include "../block/BlockVariant.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>

namespace std {
template <> struct hash<mc::Vector3i> {
    std::size_t operator()(const mc::Vector3i& s) const noexcept {
        std::size_t seed = 3;
        for (int i = 0; i < 3; ++i) {
            seed ^= s[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
}

namespace terra {
namespace block {

class BlockModel;

} // ns block

namespace render {

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    u32 texture_index;
    glm::vec3 tint;
    unsigned char ambient_occlusion;

    Vertex(glm::vec3 pos, glm::vec2 uv, u32 tex_index, glm::vec3 tint, int ambient_occlusion)
        : position(pos), uv(uv), texture_index(tex_index), tint(tint), ambient_occlusion(static_cast<unsigned char>(ambient_occlusion))
    {
    }
};

struct ChunkMeshBuildContext {
    // Store the chunk data and a border around the chunk
    mc::block::BlockPtr chunk_data[18 * 18 * 18];
    mc::Vector3i world_position;

    mc::block::BlockPtr GetBlock(const mc::Vector3i& world_pos) {
        mc::Vector3i::value_type x = world_pos.x - world_position.x + 1;
        mc::Vector3i::value_type y = world_pos.y - world_position.y + 1;
        mc::Vector3i::value_type z = world_pos.z - world_position.z + 1;

        return chunk_data[y * 18 * 18 + z * 18 + x];
    }
};

class ChunkMeshGenerator : public terra::WorldListener {
public:
    using iterator = std::unordered_map<mc::Vector3i, std::unique_ptr<terra::render::ChunkMesh>>::iterator;

    ChunkMeshGenerator(terra::World* world, const glm::vec3& camera_position);
    ~ChunkMeshGenerator();

    void OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) override;
    void OnChunkLoad(terra::ChunkPtr chunk, const terra::ChunkColumnMetadata& meta, u16 index_y) override;
    void OnChunkUnload(terra::ChunkColumnPtr chunk) override;

    void GenerateMesh(s64 chunk_x, s64 chunk_y, s64 chunk_z);
    void GenerateMesh(ChunkMeshBuildContext& context);
    void DestroyChunk(s64 chunk_x, s64 chunk_y, s64 chunk_z);

    iterator begin() { return m_ChunkMeshes.begin(); }
    iterator end() { return m_ChunkMeshes.end(); }

    void ProcessChunks();

private:
    struct ChunkMeshBuildComparator {
        const glm::vec3& position;

        ChunkMeshBuildComparator(const glm::vec3& position) : position(position) { }

        bool operator()(const std::shared_ptr<ChunkMeshBuildContext>& first_ctx, const std::shared_ptr<ChunkMeshBuildContext>& second_ctx) {
            mc::Vector3i first = first_ctx->world_position;
            mc::Vector3i second = second_ctx->world_position;

            glm::vec3 f(first.x, 0, first.z);
            glm::vec3 s(second.x, 0, second.z);

            glm::vec3 a = f - position;
            glm::vec3 b = s - position;

            return glm::dot(a, a) > glm::dot(b, b);
        }
    };

    struct VertexPush {
        mc::Vector3i pos;
        std::unique_ptr<std::vector<Vertex>> vertices;

        VertexPush(const mc::Vector3i& pos, std::unique_ptr<std::vector<Vertex>> vertices) : pos(pos), vertices(std::move(vertices)) { }
    };

    int GetAmbientOcclusion(ChunkMeshBuildContext& context, const mc::Vector3i& side1, const mc::Vector3i& side2, const mc::Vector3i& corner);
    bool IsOccluding(terra::block::BlockVariant* from_variant, terra::block::BlockFace face, mc::block::BlockPtr test_block);
    void WorkerUpdate();
    void EnqueueBuildWork(long chunk_x, int chunk_y, long chunk_z);

    std::mutex m_QueueMutex;
    PriorityQueue<std::shared_ptr<ChunkMeshBuildContext>, ChunkMeshBuildComparator> m_ChunkBuildQueue;
    std::deque<mc::Vector3i> m_ChunkPushQueue;
    std::condition_variable m_BuildCV;

    terra::World* m_World;

    std::mutex m_PushMutex;
    std::vector<std::unique_ptr<VertexPush>> m_VertexPushes;

    std::unordered_map<mc::Vector3i, std::unique_ptr<terra::render::ChunkMesh>> m_ChunkMeshes;

    bool m_Working;
    std::vector<std::thread> m_Workers;
};

} // ns render
} // ns terra

#endif
