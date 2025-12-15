#include "world/OnlineWorld.h"
#include "gui/ImGuiManager.h"

OnlineWorld::OnlineWorld() {
    ImGuiManager::GetInstance()->RegisterCallback("OWCE", [=, this]() {
        ImGui::Begin("OnlineWorld Chunk Events");

        ImGui::Text("Loaded Chunks: %d", m_chunkCount);

        ImGui::End();
    });
}

OnlineWorld::~OnlineWorld() {
    ImGuiManager::GetInstance()->UnregisterCallback("OWCE");
}

void OnlineWorld::OnChunkLoad(mc::world::ChunkPtr chunk, const mc::world::ChunkColumnMetadata& meta, u16 yIndex) {
    // Here we are getting a 16x16x16 chunk (aka a craftus cluster) at meta x, z and yIndex height
    this->m_chunkCount += 1;
    WorldListener::OnChunkLoad(chunk, meta, yIndex);
}

void OnlineWorld::OnChunkUnload(mc::world::ChunkColumnPtr chunk) {
    // Here we are unloading the entire 16x256x16 chunk column at chunk x,z
    this->m_chunkCount -= 1;
    WorldListener::OnChunkUnload(chunk);
}

void OnlineWorld::OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) {
    // A block has changed at the given position
    WorldListener::OnBlockChange(position, newBlock, oldBlock);
}