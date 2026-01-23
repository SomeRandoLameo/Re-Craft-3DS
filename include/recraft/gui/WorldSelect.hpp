#pragma once

#include <c3d/texture.h>
#include <stdbool.h>
#include <stdint.h>
#include "../entity/Player.hpp"

#include "../world/CT_World.hpp"
#include "rendering/Clouds.hpp"

typedef struct {
    std::array<char, World::NameSize> worldName;
    GeneratorSettings settings;
} WorldSelect_Result;

typedef struct {
    uint32_t lastPlayed;
    char name[World::NameSize];
    char path[256];
} WorldInfo;

static std::vector<WorldInfo> worlds;


typedef enum { WorldSelect, ConfirmDeletion, WorldOptions } MenuState;

static int scroll = 0;
static float velocity = 0.f;
static int selectedWorld = -1;

static bool clicked_play = false;
static bool clicked_new_world = false;
static bool clicked_mp_connect = false;
static bool clicked_delete_world = false;

static bool confirmed_world_options = false;
static bool canceled_world_options = false;

static bool confirmed_deletion = false;
static bool canceled_deletion = false;

static WorldGenType worldGenType = WorldGenType::SuperFlat;

static Gamemode gamemode1 = Gamemode::Survival;

static constexpr std::array<const char*, 4> gamemodestr = {"Survival", "Creative", "Adventure", "Spectator"};
static constexpr std::array<const char*, 3> worldGenTypesStr = {"Smea", "Superflat", "Empty"};

static MenuState menustate = MenuState::WorldSelect;

static float max_velocity = 20.f;

static char out_worldpath[256];
static char out_name[World::NameSize] = {'\0'};
static WorldGenType worldType;
static bool newWorld = false;


void WorldSelect_Init();
void WorldSelect_Deinit();

void WorldSelect_RenderTop(Clouds* m_clouds, int m_world_shader_uLocProjection, int eyeIndex, float iod, World* m_world,
                           C3D_Tex* m_logoTex);
void WorldSelect_RenderBot();

void WorldSelect_Update(Player* player);

void WorldSelect_ScanWorlds();
