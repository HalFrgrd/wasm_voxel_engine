#include "chunkmesh.h"

ChunkMesh::ChunkMesh(Renderer *initRenderer) : renderer(initRenderer){
    vertex_array_buffer = renderer->createVertexArrayBuffer();
    vertex_buffer = renderer->createVertexBuffer();
    index_buffer = renderer->createIndexBuffer();
    colour_buffer = renderer->createColourBuffer();
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArraysOES(1, &vertex_array_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
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

void ChunkMesh::addQuad(glm::vec4 corner1, glm::vec4 corner2, glm::vec4 corner3, glm::vec4 corner4, glm::vec3 colour, bool isBackFace ){

    if (!isBackFace) {
        indices.push_back(index_counter + 0);
        indices.push_back(index_counter + 1);
        indices.push_back(index_counter + 2);
        indices.push_back(index_counter + 0);
        indices.push_back(index_counter + 2);
        indices.push_back(index_counter + 3);
    } else {
        indices.push_back(index_counter + 2);
        indices.push_back(index_counter + 1);
        indices.push_back(index_counter + 0);
        indices.push_back(index_counter + 3);
        indices.push_back(index_counter + 2);
        indices.push_back(index_counter + 0);
    }

    addVertex(corner1, colour);
    addVertex(corner2, colour);
    addVertex(corner3, colour);
    addVertex(corner4, colour);

    index_counter += 4;
}

void ChunkMesh::bufferMesh(){
    glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
    glBufferData(GL_ARRAY_BUFFER, colours.size()*sizeof(GLfloat), &colours[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}