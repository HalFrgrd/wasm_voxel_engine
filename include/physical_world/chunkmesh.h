#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>

#include <SDL_opengles2.h>

#include "view.h"
#include "camera.h"
#include "block.h"
#include "world.h"
#include "terrain.h"

#include <atomic>


class ChunkMesh{
public:

    Renderer *renderer;
    ChunkMesh(Renderer *initRenderer);
    ~ChunkMesh();

    bool isFullyGenerated = false;
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colours;
    std::vector<unsigned int> indices;
    unsigned int index_counter = 0;

    GLuint vertex_array_buffer;
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint colour_buffer;


    void addQuad(glm::vec4 corner1,glm::vec4 corner2,glm::vec4 corner3,glm::vec4 corner4, glm::vec3 colour, bool isBackFace );
    void addVertex(glm::vec4 vertex, glm::vec3 colour);

    // Put the mesh (all the arrays) into the gpu
    void bufferMesh();
};

#endif