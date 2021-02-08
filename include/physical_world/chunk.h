#ifndef CHUNK_H
#define CHUNK_H


#define GL_GLEXT_PROTOTYPES 1

#include <emscripten.h>

#include <SDL.h>

#include <SDL_opengles2.h>

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

    Chunk(World *initWorld, GUI_Interface *initInterface, int _chunkX, int _chunkY, int _chunkZ, TerrainGenerator* terrain, Renderer* initRenderer);
    ~Chunk();

	void setChunkCoords(int x, int y, int z);
    World *my_world;
    GUI_Interface *interface;

    int chunkX;
    int chunkY;
    int chunkZ;

    ChunkMesh *mesh;
    bool lastMeshStillValid = false;
    
    // This chunk is all air, so no need to generate mesh
    bool isEmpty = true; 
    bool declaredNoDraw = false;

	static const int chunkSize = 32;

    Block::BlockType cubePositions[chunkSize*chunkSize*chunkSize];

    int flattenCoords(glm::ivec3 coords);
	int flattenCoords(int i, int j, int k);
    glm::ivec3 localPosToGlobalPos(int x, int y, int z);
	glm::ivec3 unflattenCoords(int i);
    Block::BlockType getBlockFromWorld(glm::ivec3 coords);
    Block::BlockType getBlockFromChunk(glm::ivec3 coords);

    void generateGreedyMesh();
    void renderChunk(Camera &camera);

    bool isBlockFaceVisible(glm::ivec3 blockPos, int axis, bool isBackFace);
    bool compareStep(glm::ivec3 a, glm::ivec3 b, int direction, bool isBackFace);

};



#endif