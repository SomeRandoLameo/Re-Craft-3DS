#pragma once

#include "../world/CT_World.h"
#include "../entity/Player.h"
#include "../world/WorkQueue.h"
#include "Camera.h"
#include "Cursor.h"
#include <citro3d.h>
#include <vec.h>

struct RenderStep {
	Cluster* cluster;
	Chunk* chunk;
	Direction enteredFrom;
};

struct TransparentRender {
	Cluster* cluster;
	Chunk* chunk;
};

class WorldRenderer {
public:
	WorldRenderer(Player* player, World* world, WorkQueue* workqueue, int projectionUniform);
	~WorldRenderer();

	// Delete copy constructor and assignment operator
	WorldRenderer(const WorldRenderer&) = delete;
	WorldRenderer& operator=(const WorldRenderer&) = delete;

	void Render(float iod);

private:
	void RenderWorld();

	Player* player;
	World* world;
	WorkQueue* workqueue;
	Camera cam;
	int projectionUniform;

	Cursor* cursor;

	vec_t(RenderStep) renderingQueue;
	uint8_t chunkRendered[CHUNKCACHE_SIZE][CLUSTER_PER_CHUNK][CHUNKCACHE_SIZE];
	vec_t(TransparentRender) transparentClusters;

	C3D_FogLut fogLut;

	inline bool ClusterWasRendered(int x, int y, int z) const {
		return chunkRendered[x - (world->cacheTranslationX - (CHUNKCACHE_SIZE / 2))][y]
		[z - (world->cacheTranslationZ - (CHUNKCACHE_SIZE / 2))];
	}

	inline uint8_t& ClusterRenderedRef(int x, int y, int z) {
		return chunkRendered[x - (world->cacheTranslationX - (CHUNKCACHE_SIZE / 2))][y]
		[z - (world->cacheTranslationZ - (CHUNKCACHE_SIZE / 2))];
	}
};