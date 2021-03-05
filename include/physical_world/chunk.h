#ifndef CHUNK_H
#define CHUNK_H


#define GL_GLEXT_PROTOTYPES 1

#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>

#include <pthread.h>
#include <atomic>

#include "view.h"
#include "camera.h"
#include "block.h"
#include "world.h"
#include "terrain.h"
#include "chunkmesh.h"
#include "timer.h"
#include "interface.h"

class Chunk{
public:

    Chunk(World *initWorld, GUI_Interface *initInterface, glm::ivec3 initChunkPos, TerrainGenerator* terrain, Renderer* initRenderer);
    ~Chunk();

    World *my_world;
    GUI_Interface *interface;
    TerrainGenerator *terrain;

    void initialiseTerrain();

    // Index of chunk
    glm::ivec3 chunkPos;

    ChunkMesh *mesh;
    bool lastMeshStillValid = false;
    bool isMeshInBuffer = false;
    bool terrainPopulationFinished = false;

    // Don't delete chunk when being accessed in other threads
    std::atomic<bool> doNotDelete{false};
    // Don't bother meshing / initialising this chunk, going to be deleted soon.
    // std::atomic<bool> dontBother{false};
    bool dontBother = false;
    
    // If this chunk is all air, so no need to generate mesh
    bool isEmpty = true; 
    bool declaredNoDraw = false;

	static const int chunkSize = 32;
    // const glm::ivec3 chunkDims = glm::ivec3(32,32,32);

    Block::BlockType *cubePositions = new Block::BlockType[32*32*32];

    inline int flattenCoords(glm::ivec3 coords);
	inline glm::ivec3 unflattenCoords(int i);
    inline glm::ivec3 localPosToGlobalPos(glm::ivec3 localBlockPos);
    inline Block::BlockType getBlockFromWorld(glm::ivec3 coords);
    inline Block::BlockType getBlockFromChunk(glm::ivec3 coords);

    void generateGreedyMesh();
    void renderChunk();
    void renderChunkOutline();

    bool isBlockFaceVisible(glm::ivec3 blockPos, int axis, bool isBackFace);
    bool compareStep(glm::ivec3 a, glm::ivec3 b, int direction, bool isBackFace);

};



#endif