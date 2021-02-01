#include "chunkmesh.h"

ChunkMesh::ChunkMesh(Renderer *initRenderer) : renderer(initRenderer){
    vertex_array_buffer = initRenderer->getVertexArrayBuffer();
    vertex_buffer = initRenderer->getVertexBuffer();
    colour_buffer = initRenderer->getColourBuffer();
}

void ChunkMesh::addVertex(glm::vec4 vertex, glm::vec3 colour){
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);

    colours.push_back(colour.x);
    colours.push_back(colour.y);
    colours.push_back(colour.z);
}

void ChunkMesh::addQuad(glm::vec4 quadVertices[],glm::vec3 colour, bool isBackFace ){

    if (!isBackFace) {
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[1], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[3], colour);
    } else {
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[1], colour);
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[3], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[0], colour);
    }
}
