#include "ChunkMeshGenerator.h"

#include "../assets/AssetCache.h"
#include "../math/TypeUtil.h"
#include "../block/BlockModel.h"
#include "../block/BlockState.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/quaternion.hpp>

namespace terra {
namespace render {

ChunkMeshGenerator::ChunkMeshGenerator(terra::World* world, const glm::vec3& camera_position) : m_World(world), m_ChunkBuildQueue(ChunkMeshBuildComparator(camera_position)) {
    world->RegisterListener(this);

    m_Working = true;
    unsigned int count = std::thread::hardware_concurrency() - 1;

    std::cout << "Creating " << count << " mesh worker threads." << std::endl;

    for (unsigned int i = 0; i < count; ++i) {
        m_Workers.emplace_back(&ChunkMeshGenerator::WorkerUpdate, this);
    }
}

ChunkMeshGenerator::~ChunkMeshGenerator() {
    m_Working = false;

    m_BuildCV.notify_all();

    for (auto& worker : m_Workers) {
        worker.join();
    }

    m_World->UnregisterListener(this);
}

void ChunkMeshGenerator::OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) {
    terra::ChunkColumnPtr column = m_World->GetChunk(position);
    const mc::Vector3i chunk_base(column->GetMetadata().x * 16, (position.y / 16) * 16, column->GetMetadata().z * 16);
    int chunk_x = column->GetMetadata().x;
    int chunk_y = static_cast<int>(position.y / 16);
    int chunk_z = column->GetMetadata().z;

    auto iter = m_ChunkMeshes.find(chunk_base);

    if (iter == m_ChunkMeshes.end()) {
        GenerateMesh(chunk_x, chunk_y, chunk_z);
        return;
    }

    // TODO: Incremental update somehow?
    EnqueueBuildWork(chunk_x, chunk_y, chunk_z);

    if (position.x % 16 == 0) {
        EnqueueBuildWork(chunk_x - 1, chunk_y, chunk_z);
    } else if (position.x % 16 == 15) {
        EnqueueBuildWork(chunk_x + 1, chunk_y, chunk_z);
    }

    if (position.y % 16 == 0) {
        EnqueueBuildWork(chunk_x, chunk_y - 1, chunk_z);
    } else if (position.y % 16 == 15) {
        EnqueueBuildWork(chunk_x, chunk_y + 1, chunk_z);
    }

    if (position.z % 16 == 0) {
        EnqueueBuildWork(chunk_x, chunk_y, chunk_z - 1);
    } else if (position.z % 16 == 15) {
        EnqueueBuildWork(chunk_x, chunk_y, chunk_z);
    }
}

void ChunkMeshGenerator::OnChunkLoad(terra::ChunkPtr chunk, const terra::ChunkColumnMetadata& meta, u16 index_y) {
    EnqueueBuildWork(meta.x, index_y, meta.z);

    EnqueueBuildWork(meta.x - 1, index_y, meta.z);
    EnqueueBuildWork(meta.x + 1, index_y, meta.z);
    EnqueueBuildWork(meta.x, index_y - 1, meta.z);
    EnqueueBuildWork(meta.x, index_y + 1, meta.z);
    EnqueueBuildWork(meta.x, index_y, meta.z - 1);
    EnqueueBuildWork(meta.x, index_y, meta.z + 1);
}

void ChunkMeshGenerator::EnqueueBuildWork(long chunk_x, int chunk_y, long chunk_z) {
    mc::Vector3i position(chunk_x * 16, chunk_y * 16, chunk_z * 16);

    auto iter = std::find(m_ChunkPushQueue.begin(), m_ChunkPushQueue.end(), position);

    if (iter == m_ChunkPushQueue.end()) {
        m_ChunkPushQueue.push_back(position);
    }
}

void ChunkMeshGenerator::WorkerUpdate() {
    while (m_Working) {
        if (m_ChunkBuildQueue.Empty()) {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_BuildCV.wait(lock);
        }

        std::shared_ptr<ChunkMeshBuildContext> ctx;

        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);

            if (m_ChunkBuildQueue.Empty()) continue;

            ctx = m_ChunkBuildQueue.Pop();
        }

        GenerateMesh(*ctx);
    }
}

void ChunkMeshGenerator::ProcessChunks() {
    const std::size_t kMaxMeshesPerFrame = 64;

    // Push any new chunks that were added this frame into the work queue
    for (std::size_t i = 0; i < kMaxMeshesPerFrame && !m_ChunkPushQueue.empty(); ++i) {
        mc::Vector3i chunk_base = m_ChunkPushQueue.front();
        m_ChunkPushQueue.pop_front();

        auto ctx = std::make_shared<ChunkMeshBuildContext>();

        ctx->world_position = chunk_base;
        mc::Vector3i offset_y(0, chunk_base.y, 0);

        terra::ChunkColumnPtr columns[3][3];

        columns[1][1] = m_World->GetChunk(chunk_base);

        // Cache the world data for this chunk so it can be pushed to another thread and built.
        for (s64 y = 0; y < 18; ++y) {
            for (s64 z = 0; z < 18; ++z) {
                for (s64 x = 0; x < 18; ++x) {
                    mc::Vector3i offset(x - 1, y - 1, z - 1);
                    mc::block::BlockPtr block = nullptr;

                    std::size_t x_index = (s64)std::floor(offset.x / 16.0) + 1;
                    std::size_t z_index = (s64)std::floor(offset.z / 16.0) + 1;
                    auto& column = columns[x_index][z_index];

                    if (column == nullptr) {
                        column = m_World->GetChunk(chunk_base + offset);
                    }

                    if (column != nullptr) {
                        mc::Vector3i lookup = offset + offset_y;

                        if (lookup.x < 0) {
                            lookup.x += 16;
                        } else if (lookup.x > 15) {
                            lookup.x -= 16;
                        }

                        if (lookup.z < 0) {
                            lookup.z += 16;
                        } else if (lookup.z > 15) {
                            lookup.z -= 16;
                        }

                        block = column->GetBlock(lookup);
                    }

                    ctx->chunk_data[y * 18 * 18 + z * 18 + x] = block;
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_ChunkBuildQueue.Push(ctx);
        }

        m_BuildCV.notify_one();
    }

    std::vector<std::unique_ptr<VertexPush>> pushes;

    {
        std::lock_guard<std::mutex> lock(m_PushMutex);
        pushes = std::move(m_VertexPushes);
        m_VertexPushes = std::vector<std::unique_ptr<VertexPush>>();
    }

    for (auto&& push : pushes) {
        DestroyChunk(push->pos.x / 16, push->pos.y / 16, push->pos.z / 16);

        auto&& vertices = push->vertices;

        if (vertices->empty()) continue;

        GLuint vao = 0;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        g_AssetCache->GetTextures().Bind();

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices->size(), &(*vertices)[0], GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // TextureCoord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(1);

        // TextureIndex
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, texture_index));
        glEnableVertexAttribArray(2);

        // Tint
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tint));
        glEnableVertexAttribArray(3);

        // Ambient occlusion
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, ambient_occlusion));
        glEnableVertexAttribArray(4);

        GLenum error;

        while ((error = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error when creating mesh: " << error << std::endl;
        }

        std::unique_ptr<terra::render::ChunkMesh> mesh = std::make_unique<terra::render::ChunkMesh>(vao, vbo, vertices->size());

        m_ChunkMeshes[push->pos] = std::move(mesh);
    }
}

int ChunkMeshGenerator::GetAmbientOcclusion(ChunkMeshBuildContext& context, const mc::Vector3i& side1, const mc::Vector3i& side2, const mc::Vector3i& corner) {
    int value1, value2, value_corner;

    auto bs1 = context.GetBlock(side1);
    auto bs2 = context.GetBlock(side2);
    auto bsc = context.GetBlock(corner);

    value1 = bs1 && bs1->IsSolid();
    value2 = bs2 && bs2->IsSolid();
    value_corner = bsc && bsc->IsSolid();

    if (value1 && value2) {
        return 0;
    }

    return 3 - (value1 + value2 + value_corner);
}

bool ChunkMeshGenerator::IsOccluding(terra::block::BlockVariant* from_variant, terra::block::BlockFace face, mc::block::BlockPtr test_block) {
    if (test_block == nullptr) return true;
    if (from_variant->HasRotation()) return false;

    for (auto& element : from_variant->GetModel()->GetElements()) {
        if (element.GetFace(face).cull_face == block::BlockFace::None) {
            return false;
        }
    }

    terra::block::BlockVariant* variant = g_AssetCache->GetVariant(test_block);
    if (variant == nullptr) return false;

    if (variant->HasRotation()) return false;

    terra::block::BlockModel* model = variant->GetModel();
    if (model == nullptr) return false;

    bool is_full = false;

    block::BlockFace opposite = block::get_opposite_face(face);
    auto& textures = g_AssetCache->GetTextures();
    for (auto& element : model->GetElements()) {
        auto opposite_face = element.GetFace(opposite);
        bool transparent = textures.IsTransparent(opposite_face.texture);
        bool full_extent = element.IsFullExtent();

        if (full_extent && !transparent) {
            is_full = true;
        }
    }
    
    return is_full;
}

std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
    return out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

void ApplyRotations(glm::vec3& bottom_left, glm::vec3& bottom_right, glm::vec3& top_left, glm::vec3& top_right, const glm::vec3& rotations, glm::vec3 offset = glm::vec3(0.5, 0.5, 0.5)) {
    glm::quat quat(1, 0, 0, 0);

    const float kToRads = (float)M_PI / 180.0f;

    if (rotations.z != 0) {
        quat = glm::rotate(quat, kToRads * rotations.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (rotations.y != 0) {
        quat = glm::rotate(quat, kToRads * -rotations.y, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (rotations.x != 0) {
        quat = glm::rotate(quat, kToRads * rotations.x, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    if (rotations.x != 0 || rotations.y != 0 || rotations.z != 0) {
        bottom_left = glm::vec3(quat * glm::vec4(bottom_left - offset, 1.0)) + offset;
        bottom_right = glm::vec3(quat * glm::vec4(bottom_right - offset, 1.0)) + offset;
        top_left = glm::vec3(quat * glm::vec4(top_left - offset, 1.0)) + offset;
        top_right = glm::vec3(quat * glm::vec4(top_right - offset, 1.0)) + offset;
    }
}

// TODO: Rescaling?
void ApplyRotations(glm::vec3& bottom_left, glm::vec3& bottom_right, glm::vec3& top_left, glm::vec3& top_right, const glm::vec3& variant_rotation, const block::ElementRotation& rotation) {
    glm::vec3 rotations(rotation.angle, rotation.angle, rotation.angle);

    glm::quat quat(1, 0, 0, 0);

    const float kToRads = (float)M_PI / 180.0f;

    if (variant_rotation.z != 0) {
        quat = glm::rotate(quat, kToRads * variant_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (variant_rotation.y != 0) {
        quat = glm::rotate(quat, kToRads * -variant_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (variant_rotation.x != 0) {
        quat = glm::rotate(quat, kToRads * variant_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    
    if (rotation.rescale) {
        rotations = rotations * (1.0f / std::cos(3.14159f / 4.0f) - 1.0f);
    }

    // Hadamard product to get just the one axis rotation
    rotations = rotations * (quat * rotation.axis);

    ApplyRotations(bottom_left, bottom_right, top_left, top_right, rotations, quat * (rotation.origin - glm::vec3(0.5, 0.5, 0.5)) + glm::vec3(0.5, 0.5, 0.5));
}

// TODO: Calculate occlusion under rotation
// TODO: Calculate UV under rotation for UV locked variants
void ChunkMeshGenerator::GenerateMesh(ChunkMeshBuildContext& context) {
    std::unique_ptr<std::vector<Vertex>> vertices = std::make_unique<std::vector<Vertex>>();

    vertices->reserve(12500);

    static const glm::vec3 kTints[] = {
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(137 / 255.0, 191 / 255.0, 98 / 255.0), // Grass
        glm::vec3(0.22, 0.60, 0.21), // Leaves
    };

    // Sweep through the blocks and generate vertices for the mesh
    for (int y = 0; y < 16; ++y) {
        for (int z = 0; z < 16; ++z) {
            for (int x = 0; x < 16; ++x) {
                mc::Vector3i mc_pos = context.world_position + mc::Vector3i(x, y, z);

                mc::block::BlockPtr block = context.GetBlock(mc_pos);
                if (block == nullptr) continue;

                terra::block::BlockVariant* variant = g_AssetCache->GetVariant(block);
                if (variant == nullptr) continue;

                terra::block::BlockModel* model = variant->GetModel();
                if (model == nullptr || model->GetElements().empty()) continue;

                const glm::vec3 base = terra::math::VecToGLM(mc_pos);

                mc::block::BlockPtr above = context.GetBlock(mc_pos + mc::Vector3i(0, 1, 0));
                if (!IsOccluding(variant, block::BlockFace::Up, above)) {
                    // Render the top face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, 1, 0), mc_pos + mc::Vector3i(0, 1, -1), mc_pos + mc::Vector3i(-1, 1, -1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, 1, 0), mc_pos + mc::Vector3i(0, 1, 1), mc_pos + mc::Vector3i(-1, 1, 1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 1, 0), mc_pos + mc::Vector3i(0, 1, -1), mc_pos + mc::Vector3i(1, 1, -1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 1, 0), mc_pos + mc::Vector3i(0, 1, 1), mc_pos + mc::Vector3i(1, 1, 1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::Up);
                        if (renderable.face == block::BlockFace::Up) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(from.x, to.y, from.z);
                            glm::vec3 bottom_right = glm::vec3(from.x, to.y, to.z);
                            glm::vec3 top_left = glm::vec3(to.x, to.y, from.z);
                            glm::vec3 top_right = glm::vec3(to.x, to.y, to.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_from.x, renderable.uv_from.y);
                            glm::vec2 br_uv(renderable.uv_from.x, renderable.uv_to.y);
                            glm::vec2 tr_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 tl_uv(renderable.uv_to.x, renderable.uv_from.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }

                mc::block::BlockPtr below = context.GetBlock(mc_pos - mc::Vector3i(0, 1, 0));
                if (!IsOccluding(variant, block::BlockFace::Down, below)) {
                    // Render the bottom face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, -1, 0), mc_pos + mc::Vector3i(0, -1, -1), mc_pos + mc::Vector3i(1, -1, -1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, -1, 0), mc_pos + mc::Vector3i(0, -1, 1), mc_pos + mc::Vector3i(1, -1, 1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, -1, 0), mc_pos + mc::Vector3i(0, -1, -1), mc_pos + mc::Vector3i(-1, -1, -1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, -1, 0), mc_pos + mc::Vector3i(0, -1, 1), mc_pos + mc::Vector3i(-1, -1, 1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::Down);

                        if (renderable.face == block::BlockFace::Down) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(to.x, from.y, from.z);
                            glm::vec3 bottom_right = glm::vec3(to.x, from.y, to.z);
                            glm::vec3 top_left = glm::vec3(from.x, from.y, from.z);
                            glm::vec3 top_right = glm::vec3(from.x, from.y, to.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 br_uv(renderable.uv_to.x, renderable.uv_from.y);
                            glm::vec2 tr_uv(renderable.uv_from.x, renderable.uv_from.y);
                            glm::vec2 tl_uv(renderable.uv_from.x, renderable.uv_to.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }

                mc::block::BlockPtr north = context.GetBlock(mc_pos + mc::Vector3i(0, 0, -1));
                if (!IsOccluding(variant, block::BlockFace::North, north)) {
                    // Render the north face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 0, -1), mc_pos + mc::Vector3i(0, -1, -1), mc_pos + mc::Vector3i(1, -1, -1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(0, -1, -1), mc_pos + mc::Vector3i(-1, 0, -1), mc_pos + mc::Vector3i(-1, -1, -1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(0, 1, -1), mc_pos + mc::Vector3i(1, 0, -1), mc_pos + mc::Vector3i(1, 1, -1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(0, 1, -1), mc_pos + mc::Vector3i(-1, 0, -1), mc_pos + mc::Vector3i(-1, 1, -1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::North);

                        if (renderable.face == block::BlockFace::North) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(to.x, from.y, from.z);
                            glm::vec3 bottom_right = glm::vec3(from.x, from.y, from.z);
                            glm::vec3 top_left = glm::vec3(to.x, to.y, from.z);
                            glm::vec3 top_right = glm::vec3(from.x, to.y, from.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_from.x, renderable.uv_to.y);
                            glm::vec2 br_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 tr_uv(renderable.uv_to.x, renderable.uv_from.y);
                            glm::vec2 tl_uv(renderable.uv_from.x, renderable.uv_from.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }

                mc::block::BlockPtr south = context.GetBlock(mc_pos + mc::Vector3i(0, 0, 1));
                if (!IsOccluding(variant, block::BlockFace::South, south)) {
                    // Render the south face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, 0, 1), mc_pos + mc::Vector3i(0, -1, 1), mc_pos + mc::Vector3i(-1, -1, 1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 0, 1), mc_pos + mc::Vector3i(0, -1, 1), mc_pos + mc::Vector3i(1, -1, 1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(0, 1, 1), mc_pos + mc::Vector3i(-1, 0, 1), mc_pos + mc::Vector3i(-1, 1, 1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(0, 1, 1), mc_pos + mc::Vector3i(1, 0, 1), mc_pos + mc::Vector3i(1, 1, 1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::South);

                        if (renderable.face == block::BlockFace::South) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(from.x, from.y, to.z);
                            glm::vec3 bottom_right = glm::vec3(to.x, from.y, to.z);
                            glm::vec3 top_left = glm::vec3(from.x, to.y, to.z);
                            glm::vec3 top_right = glm::vec3(to.x, to.y, to.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_from.x, renderable.uv_to.y);
                            glm::vec2 br_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 tr_uv(renderable.uv_to.x, renderable.uv_from.y);
                            glm::vec2 tl_uv(renderable.uv_from.x, renderable.uv_from.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }

                mc::block::BlockPtr east = context.GetBlock(mc_pos + mc::Vector3i(1, 0, 0));
                if (!IsOccluding(variant, block::BlockFace::East, east)) {
                    // Render the east face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 0, 1), mc_pos + mc::Vector3i(1, -1, 0), mc_pos + mc::Vector3i(1, -1, 1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, -1, 0), mc_pos + mc::Vector3i(1, 0, -1), mc_pos + mc::Vector3i(1, -1, -1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 1, 0), mc_pos + mc::Vector3i(1, 0, 1), mc_pos + mc::Vector3i(1, 1, 1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(1, 1, 0), mc_pos + mc::Vector3i(1, 0, -1), mc_pos + mc::Vector3i(1, 1, -1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::East);

                        if (renderable.face == block::BlockFace::East) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(to.x, from.y, to.z);
                            glm::vec3 bottom_right = glm::vec3(to.x, from.y, from.z);
                            glm::vec3 top_left = glm::vec3(to.x, to.y, to.z);
                            glm::vec3 top_right = glm::vec3(to.x, to.y, from.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_from.x, renderable.uv_to.y);
                            glm::vec2 br_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 tr_uv(renderable.uv_to.x, renderable.uv_from.y);
                            glm::vec2 tl_uv(renderable.uv_from.x, renderable.uv_from.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }

                mc::block::BlockPtr west = context.GetBlock(mc_pos + mc::Vector3i(-1, 0, 0));
                if (!IsOccluding(variant, block::BlockFace::West, west)) {
                    // Render the west face of the current block.
                    int obl = 3, obr = 3, otl = 3, otr = 3;

                    if (!variant->HasRotation()) {
                        obl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, -1, 0), mc_pos + mc::Vector3i(-1, 0, -1), mc_pos + mc::Vector3i(-1, -1, -1));
                        obr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, -1, 0), mc_pos + mc::Vector3i(-1, 0, 1), mc_pos + mc::Vector3i(-1, -1, 1));
                        otl = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, 1, 0), mc_pos + mc::Vector3i(-1, 0, -1), mc_pos + mc::Vector3i(-1, 1, -1));
                        otr = GetAmbientOcclusion(context, mc_pos + mc::Vector3i(-1, 1, 0), mc_pos + mc::Vector3i(-1, 0, 1), mc_pos + mc::Vector3i(-1, 1, 1));
                    }

                    for (const auto& element : model->GetElements()) {
                        block::RenderableFace renderable = element.GetFace(block::BlockFace::West);

                        if (renderable.face == block::BlockFace::West) {
                            assets::TextureHandle texture = renderable.texture;

                            const auto& from = element.GetFrom();
                            const auto& to = element.GetTo();

                            glm::vec3 bottom_left = glm::vec3(from.x, from.y, from.z);
                            glm::vec3 bottom_right = glm::vec3(from.x, from.y, to.z);
                            glm::vec3 top_left = glm::vec3(from.x, to.y, from.z);
                            glm::vec3 top_right = glm::vec3(from.x, to.y, to.z);

                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations());
                            ApplyRotations(bottom_left, bottom_right, top_left, top_right, variant->GetRotations(), element.GetRotation());

                            bottom_left += base;
                            bottom_right += base;
                            top_left += base;
                            top_right += base;

                            const glm::vec3& tint = kTints[renderable.tint_index + 1];

                            glm::vec2 bl_uv(renderable.uv_from.x, renderable.uv_to.y);
                            glm::vec2 br_uv(renderable.uv_to.x, renderable.uv_to.y);
                            glm::vec2 tr_uv(renderable.uv_to.x, renderable.uv_from.y);
                            glm::vec2 tl_uv(renderable.uv_from.x, renderable.uv_from.y);

                            int cobl = 3, cobr = 3, cotl = 3, cotr = 3;
                            if (element.ShouldShade()) {
                                cobl = obl; cobr = obr; cotl = otl; cotr = otr;
                            }

                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                            vertices->emplace_back(bottom_right, br_uv, texture, tint, cobr);
                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);

                            vertices->emplace_back(top_right, tr_uv, texture, tint, cotr);
                            vertices->emplace_back(top_left, tl_uv, texture, tint, cotl);
                            vertices->emplace_back(bottom_left, bl_uv, texture, tint, cobl);
                        }
                    }
                }
            }
        }
    }

    std::unique_ptr<VertexPush> push = std::make_unique<VertexPush>(context.world_position, std::move(vertices));

    std::lock_guard<std::mutex> lock(m_PushMutex);
    m_VertexPushes.push_back(std::move(push));
}

void ChunkMeshGenerator::GenerateMesh(s64 chunk_x, s64 chunk_y, s64 chunk_z) {
    ChunkMeshBuildContext ctx;

    ctx.world_position = mc::Vector3i(chunk_x * 16, chunk_y * 16, chunk_z * 16);

    for (int y = 0; y < 18; ++y) {
        for (int z = 0; z < 18; ++z) {
            for (int x = 0; x < 18; ++x) {
                mc::Vector3i offset(x - 1, y - 1, z - 1);
                mc::block::BlockPtr block = m_World->GetBlock(ctx.world_position + offset);

                ctx.chunk_data[y * 18 * 18 + z * 18 + x] = block;
            }
        }
    }

    GenerateMesh(ctx);
}

void ChunkMeshGenerator::OnChunkUnload(terra::ChunkColumnPtr chunk) {
    if (chunk == nullptr) return;

    for (int y = 0; y < 16; ++y) {
        DestroyChunk(chunk->GetMetadata().x, y, chunk->GetMetadata().z);
    }
}

void ChunkMeshGenerator::DestroyChunk(s64 chunk_x, s64 chunk_y, s64 chunk_z) {
    mc::Vector3i key(chunk_x * 16, chunk_y * 16, chunk_z * 16);

    auto iter = m_ChunkMeshes.find(key);
    if (iter != m_ChunkMeshes.end()) {
        iter->second->Destroy();
        m_ChunkMeshes.erase(key);
    }
}

} // ns render
} // ns terra
