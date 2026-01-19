#include "rendering/PolyGen.h"


#include "rendering/VBOCache.h"
#include "world/Direction.h"

#include "entity/Player.h"

#include <stdbool.h>

#include <3ds.h>

const WorldVertex cube_sides_lut[] = {
    // Fourth face (MX)
    // First triangle
    {{0, 0, 0}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 1}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{0, 1, 1}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    // Third face (PX)
    // First triangle
    {{1, 0, 0}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 0}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 1}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{1, 1, 1}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 0}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    // Sixth face (MY)
    // First triangle
    {{0, 0, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 0}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{1, 0, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    // Fifth face (PY)
    // First triangle
    {{0, 1, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{1, 1, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 0}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    // Second face (MZ)
    // First triangle
    {{0, 0, 0}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 0}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{1, 1, 0}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 0}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    // First face (PZ)
    // First triangle
    {{0, 0, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 0, 1}, {1, 0}, {255, 255, 255}, {0, 0, 0}},
    {{1, 1, 1}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    // Second triangle
    {{1, 1, 1}, {1, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 1, 1}, {0, 1}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
};


Block fastBlockFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, int x, int y, int z) {
    return (x < 0 || y < 0 || z < 0 || x >= Chunk::Size || y >= Chunk::Size || z >= Chunk::Size)
        ? world->GetBlock(mc::Vector3i(
              (column->x * Chunk::Size) + x,
              (chunk->y * Chunk::Size) + y,
              (column->z * Chunk::Size) + z)
                              )
        : chunk->GetBlock(x, y, z);
}

uint8_t fastMetadataFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, int x, int y, int z) {
    return (x < 0 || y < 0 || z < 0 || x >= Chunk::Size || y >= Chunk::Size || z >= Chunk::Size)
        ? world->GetMetadata(
              mc::Vector3i(
                  (column->x * Chunk::Size) + x,
                  (chunk->y * Chunk::Size) + y,
                  (column->z * Chunk::Size) + z)
                  )


        : (chunk->metadataLight[x][y][z] & 0xf);
}

void PolyGen_Init(World* world_, Player* player_) {
    world = world_;
    player = player_;


    floodfill_queue.clear();

    LightLock_Init(&updateLock);

    vboUpdates.clear();
}

void PolyGen_Deinit() {
    vboUpdates.clear();

	world->vboCache.~VBOCache();

    floodfill_queue.clear();
}

void PolyGen_Harvest(DebugUI* debugUi) {
	if (LightLock_TryLock(&updateLock) == 0) {
        debugUi->Text("VBOUpdates %d", vboUpdates.size());
		if (vboUpdates.size() > 0) {
			if (vboUpdates[0].delay++ > 2) {
                while (vboUpdates.size() > 0) {
                    VBOUpdate update = vboUpdates.back();
                    vboUpdates.pop_back();

                    ChunkColumnPtr column = world->GetChunkColumn(update.x, update.z);
                    if (column) {
                        if (column->GetChunk(update.y)->vertices > 0) {
                            world->vboCache.Free(column->GetChunk(update.y)->vbo);
                        }

                        if (column->GetChunk(update.y)->transparentVertices > 0) {
                            world->vboCache.Free(column->GetChunk(update.y)->transparentVBO);
                        }

                        column->GetChunk(update.y)->vbo = update.vbo;
                        column->GetChunk(update.y)->vertices = update.vertices;
                        column->GetChunk(update.y)->transparentVBO = update.transparentVBO;
                        column->GetChunk(update.y)->transparentVertices = update.transparentVertices;
                        column->GetChunk(update.y)->seeThrough = update.visibility;
                    }
                }
            }
		}

		LightLock_Unlock(&updateLock);
	}
}

void addFace(int x, int y, int z, Direction dir, Block block, uint8_t metadata, int ao, bool transparent) {
    if (x >= 0 && y >= 0 && z >= 0 && x < Chunk::Size && y < Chunk::Size && z < Chunk::Size) {
        faceBuffer[currentFace++] = Face{static_cast<int8_t>(x), static_cast<int8_t>(y), static_cast<int8_t>(z), dir, block, static_cast<int8_t>(ao), metadata, transparent};
        transparentFaces += transparent;
    }
}

uint16_t floodFill(World* world, ChunkColumnPtr chunk, Chunk* cluster, int x, int y, int z, Direction entrySide0, Direction entrySide1, Direction entrySide2) {
    if (floodfill_visited[x][y][z] & 1) return 0;
    uint8_t exitPoints[6] = {false};
    if (entrySide0 != Direction::Invalid) exitPoints[entrySide0] = true;
    if (entrySide1 != Direction::Invalid) exitPoints[entrySide1] = true;
    if (entrySide2 != Direction::Invalid) exitPoints[entrySide2] = true;
    floodfill_queue.clear();
    floodfill_queue.push_back(QueueElement{static_cast<int8_t>(x), static_cast<int8_t>(y), static_cast<int8_t>(z)});

    while (floodfill_queue.size() > 0) {
        QueueElement item = floodfill_queue.back();
        floodfill_queue.pop_back();


        for (int i = 0; i < 6; i++) {
            const int* offset = DirectionToOffset[i];
            int x = item.x + offset[0], y = item.y + offset[1], z = item.z + offset[2];
            if (x < 0 || y < 0 || z < 0 || x >= Chunk::Size || y >= Chunk::Size || z >= Chunk::Size) {
                exitPoints[i] = true;
            } else {
                if (!Block_Opaque(cluster->GetBlock(x,y,z), cluster->metadataLight[x][y][z] & 0xf) &&
                    !(floodfill_visited[x][y][z] & 1)) {
                    floodfill_visited[x][y][z] |= 1;
                    floodfill_queue.push_back((QueueElement){static_cast<int8_t>(x), static_cast<int8_t>(y), static_cast<int8_t>(z)});
                }
                if ((cluster->GetBlock(item.x,item.y,item.z) == Block::Air ||
                     Block_Opaque(cluster->GetBlock(x,y,z), cluster->metadataLight[x][y][z] & 0xf)) &&
                    cluster->GetBlock(x,y,z) != Block::Air) {
                    addFace(x, y, z, DirectionOpposite[i], cluster->GetBlock(x,y,z),
                            cluster->metadataLight[x][y][z] & 0xf, 0,
                            !Block_Opaque(cluster->GetBlock(x,y,z), cluster->metadataLight[x][y][z] & 0xf));
                }
            }
        }
    }

    uint16_t visiblity = 0;
    for (int i = 0; i < 6; i++) {
        if (exitPoints[i]){
            for (int j = 0; j < 6; j++){
                if (i != j && exitPoints[j]) {
                    visiblity |= ChunkSeeThrough((Direction)i, (Direction)j);
                }
            }
        }
    }
    return visiblity;
}

void PolyGen_GeneratePolygons(WorkQueue* queue, WorkerItem item, void* context) {
    for (int i = 0; i < ChunkColumn::ChunksPerColumn; i++) {
        ChunkPtr chunk = item.column->GetChunk(i);

        if (chunk->revision != chunk->vboRevision || chunk->forceVBOUpdate) {
            chunk->vboRevision = chunk->revision;
            chunk->forceVBOUpdate = false;

            currentFace = 0;
            transparentFaces = 0;

            uint16_t visibility = 0;

            memset(floodfill_visited, 0, sizeof(floodfill_visited));
            for (int x = 0; x < Chunk::Size; x += Chunk::Size - 1) {
                Direction xDir = !x ? Direction::West : Direction::East;
                for (int z = 0; z < Chunk::Size; z++) {
                    Direction zDir = Direction::Invalid;
                    if (z == 0) {
                        zDir = Direction::North;
                    } else if (z == Chunk::Size - 1) {
                        zDir = Direction::South;
                    }
                    for (int y = 0; y < Chunk::Size; y++) {
                        Direction yDir = Direction::Invalid;
                        if (y == 0) {
                            yDir = Direction::Bottom;
                        } else if (y == Chunk::Size - 1) {
                            yDir = Direction::Top;
                        }


                        if (!Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf)){
                            visibility |= floodFill(world, item.column, chunk, x, y, z, xDir, yDir, zDir);
                        }
                        Block block = fastBlockFetch(world, item.column, chunk, x + (!x ? -1 : 1), y, z);
                        uint8_t meta = fastMetadataFetch(world, item.column, chunk, x + (!x ? -1 : 1), y, z);
                        if (!Block_Opaque(block, meta) && chunk->GetBlock(x,y,z) != Block::Air) {
                            addFace(
                                x, y, z, xDir, chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf,
                                0,
                                !Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf));
                        }
                    }
                }
            }
            for (int y = 0; y < Chunk::Size; y += Chunk::Size - 1) {
                Direction yDir = !y ? Direction::Bottom : Direction::Top;
                for (int x = 0; x < Chunk::Size; x++) {
                    Direction xDir = Direction::Invalid;
                    if (x == 0){
                        xDir = Direction::West;
                    } else if (x == Chunk::Size - 1){
                        xDir = Direction::East;
                    }
                    for (int z = 0; z < Chunk::Size; z++) {
                        Direction zDir = Direction::Invalid;
                        if (z == 0) {
                            zDir = Direction::South;
                        } else if (z == Chunk::Size - 1) {
                            zDir = Direction::North;
                        }
                        if (!Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf)){
                            visibility |= floodFill(world, item.column, chunk, x, y, z, xDir, yDir, zDir);
                        }

                        Block block = fastBlockFetch(world, item.column, chunk, x, y + (!y ? -1 : 1), z);
                        uint8_t meta = fastMetadataFetch(world, item.column, chunk, x, y + (!y ? -1 : 1), z);
                        if (!Block_Opaque(block, meta) && chunk->GetBlock(x,y,z) != Block::Air) {
                            addFace(
                                x, y, z, yDir, chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf,
                                0,
                                !Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf));
                        }
                    }
                }
            }
            for (int z = 0; z < Chunk::Size; z += Chunk::Size - 1) {
                Direction zDir = !z ? Direction::North : Direction::South;
                for (int x = 0; x < Chunk::Size; x++) {
                    Direction xDir = Direction::Invalid;
                    if (x == 0) {
                        xDir = Direction::West;
                    } else if (x == Chunk::Size - 1) {
                        xDir = Direction::East;
                    }

                    for (int y = 0; y < Chunk::Size; y++) {
                        Direction yDir = Direction::Invalid;
                        if (y == 0) {
                            yDir = Direction::Bottom;
                        } else if (y == Chunk::Size - 1){
                            yDir = Direction::Top;
                        }

                        if (!Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf)){
                            visibility |= floodFill(world, item.column, chunk, x, y, z, xDir, yDir, zDir);
                        }

                        Block block = fastBlockFetch(world, item.column, chunk, x, y, z + (!z ? -1 : 1));
                        if (!Block_Opaque(block, chunk->metadataLight[x][y][z] & 0xf) &&
                            chunk->GetBlock(x,y,z) != Block::Air) {
                            addFace(
                                x, y, z, zDir, chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf,
                                0,
                                !Block_Opaque(chunk->GetBlock(x,y,z), chunk->metadataLight[x][y][z] & 0xf));
                        }
                    }
                }
            }
            int px = FastFloor(player->position.x);
            int py = FastFloor(player->position.y);
            int pz = FastFloor(player->position.z);
            if (WorldToChunkCoord(px) == item.column->x && WorldToChunkCoord(pz) == item.column->z &&
                WorldToChunkCoord(py) == i) {
                floodFill(world, item.column, chunk, WorldToLocalCoord(px), WorldToLocalCoord(py), WorldToLocalCoord(pz),
                          Direction::Invalid, Direction::Invalid, Direction::Invalid);
            }

            int transparentVertices = transparentFaces * 6;
            int verticesTotal = (currentFace * 6) - transparentVertices;
            VBOUpdate update;

            if (currentFace) {
                VBOBlock memBlock;
                if (verticesTotal > 0) {
                    memBlock = world->vboCache.Alloc(verticesTotal * sizeof(WorldVertex));
                }
                VBOBlock transparentMem;
                if (transparentFaces > 0) {
                    transparentMem = world->vboCache.Alloc(transparentVertices * sizeof(WorldVertex));
                }

                WorldVertex* opaqueData = (WorldVertex*)memBlock.memory;
                WorldVertex* transparentData = (WorldVertex*)transparentMem.memory;
                for (int j = 0; j < currentFace; j++) {
                    Face face = faceBuffer[j];

                    int offsetX = face.x + item.column->x * Chunk::Size;
                    int offsetZ = face.z + item.column->z * Chunk::Size;
                    int offsetY = face.y + i * Chunk::Size;

                    int16_t iconUV[2];
                    Block_GetTexture(mc::inventory::Slot(static_cast<s32>(face.block),0,face.metadata), face.direction, iconUV);

                    WorldVertex* data = face.transparent ? transparentData : opaqueData;
                    memcpy(data, &cube_sides_lut[face.direction * 6], sizeof(WorldVertex) * 6);

                    const int oneDivIconsPerRow = (32768 / 8);

                    uint8_t color[3];
                    Block_GetColor(face.block, face.metadata, face.direction, color);

                    for (int k = 0; k < 6; k++) {
                        data[k].xyz[0] += offsetX;
                        data[k].xyz[1] += offsetY;
                        data[k].xyz[2] += offsetZ;
                        data[k].uv[0] = (data[k].uv[0] == 1 ? (oneDivIconsPerRow - 1) : 1) + iconUV[0];
                        data[k].uv[1] = (data[k].uv[1] == 1 ? (oneDivIconsPerRow - 1) : 1) + iconUV[1];

                        data[k].rgb[0] = color[0];
                        data[k].rgb[1] = color[1];
                        data[k].rgb[2] = color[2];
                    }
                    if (face.transparent) {
                        transparentData += 6;
                    } else {
                        opaqueData += 6;
                    }
                }

                update.vbo = memBlock;
                update.transparentVBO = transparentMem;
            }

            update.x = item.column->x;
            update.y = i;
            update.z = item.column->z;
            update.vertices = verticesTotal;
            update.delay = 0;
            update.visibility = visibility;
            update.transparentVertices = transparentVertices;

            LightLock_Lock(&updateLock);
            vboUpdates.push_back(update);
            LightLock_Unlock(&updateLock);
        }
    }
    item.column->displayRevision = item.column->revision;
    item.column->forceVBOUpdate = false;
}