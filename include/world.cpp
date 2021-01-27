#include "world.h"
#include "chunk.h"

World::World() {


    for(int x = 0; x< 5; x ++){
        for(int z = 0; z<5; z++){
            Chunk* new_chunk = new Chunk(this,x,0,z, &terrain);
            worldChunks.push_back(new_chunk);
        }
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

    if (0<= chunkCoords.x  && chunkCoords.x < 5 &&
        0 == chunkCoords.y &&
        0<= chunkCoords.z && chunkCoords.z < 5 ){
            return worldChunks[chunkCoords.x + 5* chunkCoords.z]->getBlockFromChunk(getInternalChunkCoordsFromBlock(blockX, blockY, blockZ));
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



    for (int i = 0; i < worldChunks.size(); i++)
    {
        worldChunks[i]->renderChunk(renderer, camera);
    }

    SDL_GL_SwapWindow(renderer.window);

}