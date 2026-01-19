#include "world/savegame/SaveManager.h"

#include <dirent.h>
#include <unistd.h>

#include <mpack/mpack.h>

#include "gui/DebugUI.h"
#include "misc/Crash.h"

#define mkdirFlags (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)

#define mpack_elvis(node, key, typ, default_) \
	((mpack_node_type(mpack_node_map_cstr_optional((node), (key))) != mpack_type_nil) ? mpack_node_ ## typ (mpack_node_map_cstr_optional((node), (key))) : (default_))


void SaveManager::InitFileSystem() {
    mkdir("sdmc:/" SAVE_DIR, mkdirFlags);
    mkdir("sdmc:/" SAVE_DIR "/saves", mkdirFlags);
}

void SaveManager::Init(Player* player, World* world) {
    m_player = player;
    m_world = world;
}

void SaveManager::Load(char* path) {
    char buffer[256];

    sprintf(buffer, "sdmc:/" SAVE_DIR "/saves/%s", path);
    mkdir(buffer, mkdirFlags);
    chdir(buffer);

    mkdir("superchunks", mkdirFlags);

    if (access("level.mp", F_OK) != -1) {
        mpack_tree_t levelTree;
        mpack_tree_init_file(&levelTree, "level.mp", 0);
        mpack_node_t root = mpack_tree_root(&levelTree);

        mpack_node_copy_utf8_cstr(mpack_node_map_cstr(root, "name"), m_world->name, sizeof(m_world->name));

        mpack_node_t worldTypeNode = mpack_node_map_cstr_optional(root, "worldType");
        if (mpack_node_type(worldTypeNode) != mpack_type_nil){
            m_world->genSettings.type = (WorldGenType)mpack_node_uint(worldTypeNode);
        } else {
            m_world->genSettings.type = WorldGenType::SuperFlat;
        }

        mpack_node_t playerNode = mpack_node_array_at(mpack_node_map_cstr(root, "players"), 0);

        m_player->position.x = mpack_node_double(mpack_node_map_cstr(playerNode, "x"));
        m_player->position.y = mpack_node_double(mpack_node_map_cstr(playerNode, "y")) + 0.1f;
        m_player->position.z = mpack_node_double(mpack_node_map_cstr(playerNode, "z"));

        m_player->spawnset = mpack_node_bool(mpack_node_map_cstr(playerNode,"ss"));
        if (mpack_node_bool(mpack_node_map_cstr(playerNode, "ss"))){
            m_player->spawnPos.x = mpack_node_double(mpack_node_map_cstr(playerNode, "sx"));
            m_player->spawnPos.y = mpack_node_double(mpack_node_map_cstr(playerNode, "sy"));
            m_player->spawnPos.z = mpack_node_double(mpack_node_map_cstr(playerNode, "sz"));
            m_player->spawnset = mpack_node_bool(mpack_node_map_cstr(playerNode,"ss"));
        }

        m_player->gamemode = mpack_node_int(mpack_node_map_cstr(playerNode,"gamemode"));
        //use this optional part for "old" version of saved worlds
        mpack_node_t hpNode = mpack_node_map_cstr_optional(playerNode, "hp");
        if (mpack_node_type(hpNode) != mpack_type_nil) {
            m_player->hp = mpack_node_int(mpack_node_map_cstr(playerNode, "hp"));
        } else {
            m_player->hp = 20;
        }
        mpack_node_t hungerNode = mpack_node_map_cstr_optional(playerNode, "hunger");
        if (mpack_node_type(hungerNode) != mpack_type_nil) {
            m_player->hunger = mpack_node_int(mpack_node_map_cstr(playerNode, "hunger"));
        } else {
            m_player->hunger = 20;
        }
        m_player->pitch = mpack_node_float(mpack_node_map_cstr(playerNode, "pitch"));
        m_player->yaw = mpack_node_float(mpack_node_map_cstr(playerNode, "yaw"));

        m_player->flying = mpack_elvis(playerNode, "flying", bool, false);
        m_player->crouching = mpack_elvis(playerNode, "crouching", bool, false);
        //player->cheats = mpack_elvis(playerNode, "cheats", bool, true);

        mpack_error_t err = mpack_tree_destroy(&levelTree);
        if (err != mpack_ok) {
            Crash("Mpack error %d while loading world manifest %s", err, path);
        }
    }
}

void SaveManager::Unload() {
    mpack_writer_t writer;
    mpack_writer_init_file(&writer, "level.mp");
    mpack_start_map(&writer, 3);

        mpack_write_cstr(&writer, "name");
        mpack_write_cstr(&writer, m_world->GetName());

        mpack_write_cstr(&writer, "players");
        mpack_start_array(&writer, 1);
            mpack_start_map(&writer, 14);


                mpack_write_cstr(&writer, "x");
                mpack_write_double(&writer, m_player->position.x);

                mpack_write_cstr(&writer, "y");
                mpack_write_double(&writer, m_player->position.y);

                mpack_write_cstr(&writer, "z");
                mpack_write_double(&writer, m_player->position.z);

                mpack_write_cstr(&writer, "sx");
                mpack_write_double(&writer, m_player->spawnPos.x);

                mpack_write_cstr(&writer, "sy");
                mpack_write_double(&writer, m_player->spawnPos.y);

                mpack_write_cstr(&writer, "sz");
                mpack_write_double(&writer, m_player->spawnPos.z);

                mpack_write_cstr(&writer, "ss");
                mpack_write_bool(&writer, m_player->spawnset);


                mpack_write_cstr(&writer, "hp");
                mpack_write_int(&writer, m_player->hp);

                mpack_write_cstr(&writer, "hunger");
                mpack_write_int(&writer, m_player->hunger);

                mpack_write_cstr(&writer, "gamemode");
                mpack_write_int(&writer, m_player->gamemode);

                /*mpack_write_cstr(&writer, "cheats");
                mpack_write_bool(&writer, m_player->cheats);*/

                mpack_write_cstr(&writer, "pitch");
                mpack_write_float(&writer, m_player->pitch);

                mpack_write_cstr(&writer, "yaw");
                mpack_write_float(&writer, m_player->yaw);

                mpack_write_cstr(&writer, "flying");
                mpack_write_bool(&writer, m_player->flying);

                mpack_write_cstr(&writer, "crouching");
                mpack_write_bool(&writer, m_player->crouching);

            mpack_finish_map(&writer);
        mpack_finish_array(&writer);

        mpack_write_cstr(&writer, "worldType");
        mpack_write_uint(&writer, m_world->GetGenSettings().type);

    mpack_finish_map(&writer);

    mpack_error_t err = mpack_writer_destroy(&writer);
    if (err != mpack_ok) {
        Crash("Mpack error %d while saving world manifest", err);
    }

    for (auto & superchunk : m_superchunks) {
        SuperChunk_Deinit(superchunk);
        Amy::Free(superchunk);
    }
    m_superchunks.clear();
}

SuperChunk* SaveManager::FetchSuperChunk(int x, int z) {
    for (int i = 0; i < m_superchunks.size(); i++) {
        if (m_superchunks[i]->x == x && m_superchunks[i]->z == z) {
            return m_superchunks[i];
        }
    }
    auto* superchunk = (SuperChunk*)Amy::Malloc(sizeof(SuperChunk));
    SuperChunk_Init(superchunk, x, z);
    m_superchunks.push_back(superchunk);
    svcSleepThread(50000);
    return superchunk;
}

void SaveManager::LoadChunk(WorkQueue* queue, WorkerItem item) {
    int x = ChunkToSuperChunkCoord(item.column->x);
    int z = ChunkToSuperChunkCoord(item.column->z);
    SuperChunk* superchunk = FetchSuperChunk(x, z);

    SuperChunk_LoadChunk(superchunk, item.column);
}

void SaveManager::SaveChunk(WorkQueue* queue, WorkerItem item) {
    int x = ChunkToSuperChunkCoord(item.column->x);
    int z = ChunkToSuperChunkCoord(item.column->z);

    SuperChunk* superchunk = FetchSuperChunk(x, z);

    SuperChunk_SaveChunk(superchunk, item.column);
}

void SaveManager::LoadChunkCallback(WorkQueue* queue, WorkerItem item, void* context) {
    auto* mgr = static_cast<SaveManager*>(context);
    mgr->LoadChunk(queue, item);
}

void SaveManager::SaveChunkCallback(WorkQueue* queue, WorkerItem item, void* context) {
    auto* mgr = static_cast<SaveManager*>(context);
    mgr->SaveChunk(queue, item);
}
