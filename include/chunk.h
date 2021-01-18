#ifndef CHUNK_H
#define CHUNK_H


#include <SDL.h>

#include <SDL_opengles2.h>

#include "view.h"
#include "camera.h"
#include "block.h"

class Chunk{
public:

    Chunk();
    
	void setChunkCoords(int x, int y, int z);
    int chunkX;
    int chunkY;
    int chunkZ;

	static const int chunkSize = 8;

    Block::BlockType cubePositions[chunkSize*chunkSize*chunkSize];


	int flattenCoords(int i, int j, int k);
	glm::vec3 unflattenCoords(int i);

    void renderChunk(Renderer &renderer, Camera &camera);

};

#endif