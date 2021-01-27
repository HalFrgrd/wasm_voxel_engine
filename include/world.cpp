#include "world.h"
#include "chunk.h"

World::World() {

    for (int i = 0; i < 16; i++)
    {
        Chunk* new_chunk = new Chunk();
        worldChunks.push_back(new_chunk);
        worldChunks[i]->setChunkCoords(i%4,0,i/4);
        worldChunks[i]->my_world = this;
    }
}


glm::ivec3 getChunkCoordsFromBlock(int blockX, int blockY, int blockZ){
        
    return glm::ivec3(blockX/Chunk::chunkSize,blockY/Chunk::chunkSize,blockZ/Chunk::chunkSize);
}

glm::ivec3 getInternalChunkCoordsFromBlock(int blockX, int blockY, int blockZ){
    return glm::ivec3(blockX%Chunk::chunkSize,blockY%Chunk::chunkSize,blockZ%Chunk::chunkSize);
}

Block::BlockType World::worldgetBlockFromWorld(int blockX, int blockY, int blockZ){
    glm::ivec3 chunkCoords = getChunkCoordsFromBlock(blockX, blockY, blockZ);

    if (0<= chunkCoords.x  && chunkCoords.x < 4 &&
        0 == chunkCoords.y &&
        0<= chunkCoords.z && chunkCoords.z < 4 ){
            return worldChunks[chunkCoords.x + 4* chunkCoords.z]->getBlockFromChunk(getInternalChunkCoordsFromBlock(blockX, blockY, blockZ));
        }
    return Block::BLOCK_AIR;
}

void World::render(Renderer &renderer, Camera &camera){

    renderer.my_shader.setMat4("view", camera.GetViewMatrix());


    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)renderer.screen_width / (float)renderer.screen_height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::mat4(1.0f);
    renderer.my_shader.setMat4("projection", projection);

    // Set background color
    glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
    //https://learnopengl.com/Getting-started/Coordinate-Systems read the bit about depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    for (int i = 0; i < 3; i++)
    {
        worldChunks[i]->renderChunk(renderer, camera);
    }

    SDL_GL_SwapWindow(renderer.window);

}