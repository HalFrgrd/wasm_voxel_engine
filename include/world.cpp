#include "world.h"
#include "chunk.h"

World::World(Renderer *initRenderer): renderer(initRenderer) {

    // printf("Started chunk generation");
    // for(int x = 0; x< 5; x ++){
    //     for(int z = 0; z<5; z++){
    //         Chunk* new_chunk = new Chunk(this,x,0,z, &terrain, initRenderer);
    //         worldChunks[glm::ivec3(x,0,z)] = new_chunk;
    //         // worldChunks.push_back(new_chunk);
    //     }
    // }
    // printf("Finished chunk generation");
    
}

glm::ivec3 getChunkCoordsFromBlock(int blockX, int blockY, int blockZ){
    return glm::ivec3(blockX/Chunk::chunkSize,blockY/Chunk::chunkSize,blockZ/Chunk::chunkSize);
}

glm::ivec3 getChunkCoordsFromBlock(glm::ivec3 block_pos){
    return getChunkCoordsFromBlock(block_pos.x,block_pos.y,block_pos.z);
}


glm::ivec3 getInternalChunkCoordsFromBlock(int blockX, int blockY, int blockZ){
    return glm::ivec3(blockX%Chunk::chunkSize,blockY%Chunk::chunkSize,blockZ%Chunk::chunkSize);
}

Block::BlockType World::worldgetBlockFromWorld(int blockX, int blockY, int blockZ){
    glm::ivec3 chunkCoords = getChunkCoordsFromBlock(blockX, blockY, blockZ);

    // if (0<= chunkCoords.x  && chunkCoords.x < 5 &&
    //     0 == chunkCoords.y &&
    //     0<= chunkCoords.z && chunkCoords.z < 5 ){
    //         return worldChunks[chunkCoords.x + 5* chunkCoords.z]->getBlockFromChunk(getInternalChunkCoordsFromBlock(blockX, blockY, blockZ));
    //     }

    if (worldChunks.find(chunkCoords) != worldChunks.end()){

        return worldChunks[chunkCoords]->getBlockFromChunk(getInternalChunkCoordsFromBlock(blockX, blockY, blockZ));

    }
    return Block::BLOCK_AIR;
}

float maxivec3(glm::ivec3 v){
    return std::max ( std::max(v.x, v.y), v.z);
}

void World::removeFarChunks(glm::vec3 block_position_to_render_around){
    glm::ivec3 chunk_position_to_render_around = getChunkCoordsFromBlock(block_position_to_render_around);
    

    for (auto chunk_pair = worldChunks.cbegin(); chunk_pair != worldChunks.cend(); ) {
        if( glm::compMax( glm::abs(chunk_pair->first - chunk_position_to_render_around)) > radius + 1 ){
            printf("Removing chunk with chunk index:  %d %d %d  \n", chunk_pair->first.x,chunk_pair->first.y,chunk_pair->first.z);
            // Will this clean up the chunk object?
            delete worldChunks[chunk_pair->first];
            worldChunks.erase(chunk_pair++);
        }else{
            ++chunk_pair;
        }
    }
}

void World::addNewChunks(glm::vec3 block_position_to_render_around){

    glm::ivec3 chunk_position_to_render_around = getChunkCoordsFromBlock(block_position_to_render_around);
    glm::ivec3 chunk_offset = glm::ivec3(0);


    for(chunk_offset.x = -radius; chunk_offset.x < radius+1; chunk_offset.x++){
    for(chunk_offset.y = -radius; chunk_offset.y < radius+1; chunk_offset.y++){
    for(chunk_offset.z = -radius; chunk_offset.z < radius+1; chunk_offset.z++){
        
        glm::ivec3 chunk_to_add_pos = chunk_position_to_render_around + chunk_offset;

        if(worldChunks.find(chunk_to_add_pos) == worldChunks.end()){
            Chunk* new_chunk = new Chunk(this,chunk_to_add_pos.x,chunk_to_add_pos.y,chunk_to_add_pos.z, &terrain, renderer);
            worldChunks[chunk_to_add_pos] = new_chunk;
            printf("Adding chunk with chunk index:  %d %d %d  \n", chunk_to_add_pos.x,chunk_to_add_pos.y,chunk_to_add_pos.z);

        }

        // std::cout<<worldChunks.size()<<std::endl;

    }   
    }   
    }
}

void World::cleanStoredChunks(glm::vec3 block_position_to_render_around) {
    removeFarChunks(block_position_to_render_around);
    addNewChunks(block_position_to_render_around);
}

void World::render( Camera &camera){

    renderer->my_shader.setMat4("view", camera.GetViewMatrix());


    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)renderer->screen_width / (float)renderer->screen_height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::mat4(1.0f);
    renderer->my_shader.setMat4("projection", projection);

    // Set background color
    glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
    //https://learnopengl.com/Getting-started/Coordinate-Systems read the bit about depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    cleanStoredChunks(camera.Position / CAMERA_TO_WORLD_POS_SCALE);

    for (auto& chunk_pair : worldChunks){
        chunk_pair.second->renderChunk(camera);
    }

    SDL_GL_SwapWindow(renderer->window);

}