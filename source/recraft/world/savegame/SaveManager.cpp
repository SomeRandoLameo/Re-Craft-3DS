#include "world/savegame/SaveManager.h"

#include <dirent.h>
#include <unistd.h>

#include <mpack/mpack.h>

#include "gui/DebugUI.h"
#include "misc/Crash.h"

#define mkdirFlags (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)

#define mpack_elvis(node, key, typ, default_) \
	((mpack_node_type(mpack_node_map_cstr_optional((node), (key))) != mpack_type_nil) ? mpack_node_ ## typ (mpack_node_map_cstr_optional((node), (key))) : (default_))

SaveManager::SaveManager() : player(nullptr), world(nullptr) {
}

SaveManager::~SaveManager() {
    Deinit();
}

void SaveManager::InitFileSystem() {
    mkdir("sdmc:/craftus_redesigned", mkdirFlags);
    mkdir("sdmc:/craftus_redesigned/saves", mkdirFlags);
}

void SaveManager::Init(Player* player_) {
    player = player_;
    world = player->world;
}

void SaveManager::Deinit() {
}

void SaveManager::Load(char* path) {
    char buffer[256];

    sprintf(buffer, "sdmc:/craftus_redesigned/saves/%s", path);
    mkdir(buffer, mkdirFlags);
    chdir(buffer);

    mkdir("superchunks", mkdirFlags);

    if (access("level.mp", F_OK) != -1) {
        mpack_tree_t levelTree;
        mpack_tree_init_file(&levelTree, "level.mp", 0);
        mpack_node_t root = mpack_tree_root(&levelTree);

        mpack_node_copy_utf8_cstr(mpack_node_map_cstr(root, "name"), world->name, sizeof(world->name));

        mpack_node_t worldTypeNode = mpack_node_map_cstr_optional(root, "worldType");
        if (mpack_node_type(worldTypeNode) != mpack_type_nil)
            world->genSettings.type = (WorldGenType)mpack_node_uint(worldTypeNode);
        else
            world->genSettings.type = WorldGen_SuperFlat;

        mpack_node_t playerNode = mpack_node_array_at(mpack_node_map_cstr(root, "players"), 0);

        player->position.x = mpack_node_double(mpack_node_map_cstr(playerNode, "x"));
        player->position.y = mpack_node_double(mpack_node_map_cstr(playerNode, "y")) + 0.1f;
        player->position.z = mpack_node_double(mpack_node_map_cstr(playerNode, "z"));

        player->spawnset = mpack_node_bool(mpack_node_map_cstr(playerNode,"ss"));
        if (mpack_node_bool(mpack_node_map_cstr(playerNode, "ss"))){
            player->spawnPos.x = mpack_node_double(mpack_node_map_cstr(playerNode, "sx"));
            player->spawnPos.y = mpack_node_double(mpack_node_map_cstr(playerNode, "sy"));
            player->spawnPos.z = mpack_node_double(mpack_node_map_cstr(playerNode, "sz"));
            player->spawnset = mpack_node_bool(mpack_node_map_cstr(playerNode,"ss"));
        }

        player->gamemode = mpack_node_int(mpack_node_map_cstr(playerNode,"gamemode"));
        //use this optional part for "old" version of saved worlds
        mpack_node_t hpNode = mpack_node_map_cstr_optional(playerNode, "hp");
        if (mpack_node_type(hpNode) != mpack_type_nil)
        {
            player->hp = mpack_node_int(mpack_node_map_cstr(playerNode, "hp"));
        }
        else
        {
            player->hp = 20;
        }
        mpack_node_t hungerNode = mpack_node_map_cstr_optional(playerNode, "hunger");
        if (mpack_node_type(hungerNode) != mpack_type_nil)
        {
            player->hunger = mpack_node_int(mpack_node_map_cstr(playerNode, "hunger"));
        }
        else
        {
            player->hunger = 20;
        }
        player->pitch = mpack_node_float(mpack_node_map_cstr(playerNode, "pitch"));
        player->yaw = mpack_node_float(mpack_node_map_cstr(playerNode, "yaw"));

        player->flying = mpack_elvis(playerNode, "flying", bool, false);
        player->crouching = mpack_elvis(playerNode, "crouching", bool, false);
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
        mpack_write_cstr(&writer, world->GetName());

        mpack_write_cstr(&writer, "players");
        mpack_start_array(&writer, 1);
            mpack_start_map(&writer, 14);


                mpack_write_cstr(&writer, "x");
                mpack_write_double(&writer, player->position.x);

                mpack_write_cstr(&writer, "y");
                mpack_write_double(&writer, player->position.y);

                mpack_write_cstr(&writer, "z");
                mpack_write_double(&writer, player->position.z);

                mpack_write_cstr(&writer, "sx");
                mpack_write_double(&writer,player->spawnPos.x);

                mpack_write_cstr(&writer, "sy");
                mpack_write_double(&writer,player->spawnPos.y);

                mpack_write_cstr(&writer, "sz");
                mpack_write_double(&writer,player->spawnPos.z);

                mpack_write_cstr(&writer, "ss");
                mpack_write_bool(&writer,player->spawnset);


                mpack_write_cstr(&writer, "hp");
                mpack_write_int(&writer, player->hp);

                mpack_write_cstr(&writer, "hunger");
                mpack_write_int(&writer, player->hunger);

                mpack_write_cstr(&writer, "gamemode");
                mpack_write_int(&writer, player->gamemode);

                /*mpack_write_cstr(&writer, "cheats");
                mpack_write_bool(&writer, player->cheats);*/

                mpack_write_cstr(&writer, "pitch");
                mpack_write_float(&writer, player->pitch);

                mpack_write_cstr(&writer, "yaw");
                mpack_write_float(&writer, player->yaw);

                mpack_write_cstr(&writer, "flying");
                mpack_write_bool(&writer, player->flying);

                mpack_write_cstr(&writer, "crouching");
                mpack_write_bool(&writer, player->crouching);

            mpack_finish_map(&writer);
        mpack_finish_array(&writer);

        mpack_write_cstr(&writer, "worldType");
        mpack_write_uint(&writer, world->GetGenSettings().type);

    mpack_finish_map(&writer);

    mpack_error_t err = mpack_writer_destroy(&writer);
    if (err != mpack_ok) {
        Crash("Mpack error %d while saving world manifest", err);
    }

    for (auto & superchunk : superchunks) {
        SuperChunk_Deinit(superchunk);
        free(superchunk);
    }
    superchunks.clear();
}

SuperChunk* SaveManager::FetchSuperChunk(int x, int z) {
    for (int i = 0; i < superchunks.size(); i++) {
        if (superchunks[i]->x == x && superchunks[i]->z == z) {
            return superchunks[i];
        }
    }
    auto* superchunk = (SuperChunk*)malloc(sizeof(SuperChunk));
    SuperChunk_Init(superchunk, x, z);
    superchunks.push_back(superchunk);
    svcSleepThread(50000);
    return superchunk;
}

void SaveManager::LoadChunk(WorkQueue* queue, WorkerItem item) {
    int x = ChunkToSuperChunkCoord(item.chunk->x);
    int z = ChunkToSuperChunkCoord(item.chunk->z);
    SuperChunk* superchunk = FetchSuperChunk(x, z);

    SuperChunk_LoadChunk(superchunk, item.chunk);
}

void SaveManager::SaveChunk(WorkQueue* queue, WorkerItem item) {
    int x = ChunkToSuperChunkCoord(item.chunk->x);
    int z = ChunkToSuperChunkCoord(item.chunk->z);

    SuperChunk* superchunk = FetchSuperChunk(x, z);

    SuperChunk_SaveChunk(superchunk, item.chunk);
}

// Static callback wrappers for WorkQueue
void SaveManager::LoadChunkCallback(WorkQueue* queue, WorkerItem item, void* context) {
    auto* mgr = static_cast<SaveManager*>(context);
    mgr->LoadChunk(queue, item);
}

void SaveManager::SaveChunkCallback(WorkQueue* queue, WorkerItem item, void* context) {
    auto* mgr = static_cast<SaveManager*>(context);
    mgr->SaveChunk(queue, item);
}