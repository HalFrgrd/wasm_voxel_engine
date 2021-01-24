#ifndef WORLD_H
#define WORLD_H

// #include "chunk.h"
#include "view.h"
#include "camera.h"
#include "block.h"

class Chunk;

class World{
public:
    std::vector<Chunk*> worldChunks;

    int test = 3;
    
    World();
    void render(Renderer &renderer, Camera &camera);
    Block::BlockType worldgetBlockFromWorld(int blockX, int blockY, int blockZ);

    Chunk* getChunk(int chunkX, int chunkY, int chunkZ);

    
};

#endif