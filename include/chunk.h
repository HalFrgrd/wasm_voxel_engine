#ifndef CHUNK_H
#define CHUNK_H


#include <SDL.h>

#include <SDL_opengles2.h>

#include "view.h"
#include "camera.h"
#include "block.h"
#include "world.h"


class ChunkMesh{
public:
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colours;

    void addQuad(glm::vec4 quadVertices[], glm::vec3 colour, bool isBackFace );
    void addVertex(glm::vec4 vertex, glm::vec3 colour);

};

class Chunk{
public:

    Chunk(World& initWorld);
    
	void setChunkCoords(int x, int y, int z);
    World &my_world;
    int chunkX;
    int chunkY;
    int chunkZ;

    ChunkMesh mesh;

	static const int chunkSize = 8;

    Block::BlockType cubePositions[chunkSize*chunkSize*chunkSize];

    int flattenCoords(glm::vec3 coords);
	int flattenCoords(int i, int j, int k);
	glm::vec3 unflattenCoords(int i);
    Block::BlockType getBlockFromWorld(glm::vec3 coords);
    Block::BlockType getBlockFromChunk(glm::vec3 coords);

    void renderChunk(Renderer &renderer, Camera &camera);

    bool isBlockFaceVisible(glm::vec3 blockPos, int axis, bool isBackFace);
    bool compareStep(glm::vec3 a, glm::vec3 b, int direction, bool isBackFace);



};



#endif