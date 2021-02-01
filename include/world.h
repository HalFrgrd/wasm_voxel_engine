#ifndef WORLD_H
#define WORLD_H

// #include "chunk.h"
#include "view.h"
#include "camera.h"
#include "block.h"
#include "terrain.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>

class Chunk;

class Chunk;

class World{
public:
    std::unordered_map<glm::ivec3, Chunk*> worldChunks;
    TerrainGenerator terrain;
    
    World(Renderer *initRenderer);
    Renderer *renderer;
    void render(Camera &camera);
    Block::BlockType worldgetBlockFromWorld(int blockX, int blockY, int blockZ);

    Chunk* getChunk(int chunkX, int chunkY, int chunkZ);

    
};

#endif