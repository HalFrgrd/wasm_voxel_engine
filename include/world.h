#ifndef WORLD_H
#define WORLD_H

// #include "chunk.h"
#include "view.h"
#include "camera.h"
#include "block.h"
#include "terrain.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/component_wise.hpp>

class Chunk;

class Chunk;

class World{
public:
    std::unordered_map<glm::ivec3, Chunk*> worldChunks;
    TerrainGenerator terrain;
    
    World(Renderer *initRenderer);
    Renderer *renderer;
    void render(Camera &camera);

    const static int radius = 1;
    void removeFarChunks(glm::vec3 block_position_to_render_around);
    void addNewChunks(glm::vec3 block_position_to_render_around);
    void cleanStoredChunks(glm::vec3 block_position_to_render_around);

    Block::BlockType worldgetBlockFromWorld(int blockX, int blockY, int blockZ);

    Chunk* getChunk(int chunkX, int chunkY, int chunkZ);

    
};

#endif