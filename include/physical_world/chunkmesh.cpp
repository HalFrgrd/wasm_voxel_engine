#include "chunkmesh.h"

ChunkMesh::ChunkMesh(Renderer *initRenderer) : renderer(initRenderer){
    vertex_array_buffer = initRenderer->getVertexArrayBuffer();
    vertex_buffer = initRenderer->getVertexBuffer();
    colour_buffer = initRenderer->getColourBuffer();
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArraysOES(1, &vertex_array_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &colour_buffer);
}

void ChunkMesh::addVertex(glm::vec4 vertex, glm::vec3 colour){
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);

    colours.push_back(colour.x);
    colours.push_back(colour.y);
    colours.push_back(colour.z);
}

void ChunkMesh::addQuad(glm::vec4 corner1, glm::vec4 corner2,glm::vec4 corner3,glm::vec4 corner4,glm::vec3 colour, bool isBackFace ){

    if (!isBackFace) {
        addVertex(corner1, colour);
        addVertex(corner2, colour);
        addVertex(corner3, colour);
        addVertex(corner1, colour);
        addVertex(corner3, colour);
        addVertex(corner4, colour);
    } else {
        addVertex(corner3, colour);
        addVertex(corner2, colour);
        addVertex(corner1, colour);
        addVertex(corner4, colour);
        addVertex(corner3, colour);
        addVertex(corner1, colour);
    }
}
