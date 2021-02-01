#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <SDL.h>

#include <SDL_opengles2.h>

#include "view.h"
#include "camera.h"
#include "block.h"
#include "world.h"
#include "terrain.h"


class ChunkMesh{
public:

    ChunkMesh(Renderer *initRenderer);
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colours;

    GLuint vertex_array_buffer;
    GLuint vertex_buffer;
    GLuint colour_buffer;

    Renderer *renderer;

    void addQuad(glm::vec4 quadVertices[], glm::vec3 colour, bool isBackFace );
    void addVertex(glm::vec4 vertex, glm::vec3 colour);

};

#endif