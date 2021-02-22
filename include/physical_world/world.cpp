#include "world.h"
#include "chunk.h"

World::World(Renderer *initRenderer, GUI_Interface *initInterface): 
    renderer(initRenderer),
    interface(initInterface) {

    // printf("Started chunk generation");
    // for(int x = 0; x< 5; x ++){
    //     for(int z = 0; z<5; z++){
    //         Chunk* new_chunk = new Chunk(this,x,0,z, &terrain, initRenderer);
    //         worldChunks[glm::ivec3(x,0,z)] = new_chunk;
    //         // worldChunks.push_back(new_chunk);
    //     }
    // }
    // printf("Finished chunk generation");

    // Spin up chunk meshing thread
    int ret;
    printf("In main: creating chunk meshing thread\n");
    ret =  pthread_create(&chunk_mesh_gen_thread, NULL, &on_thread_gen_mesh_helper, this);
    if(ret != 0) {
        printf("Error: pthread_create() failed for chunk meshing thread\n");
    }

    // Spin up chunk initialising thread
    printf("In main: creating chunk initialising thread\n");
    ret =  pthread_create(&chunk_init_thread, NULL, &on_thread_chunk_init_helper, this);
    if(ret != 0) {
        printf("Error: pthread_create() failed for chunk init thread\n");
    }

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



    // glm::ivec3 chunkCoords = getChunkCoordsFromBlock(blockX, blockY, blockZ);

    // if (worldChunks.find(chunkCoords) != worldChunks.end()){

    //     return worldChunks[chunkCoords]->getBlockFromChunk(getInternalChunkCoordsFromBlock(blockX, blockY, blockZ));

    // }
    return terrain.getBlock(glm::ivec3(blockX,blockY,blockZ));
    // return Block::BLOCK_AIR;
}

float maxivec3(glm::ivec3 v){
    return std::max ( std::max(v.x, v.y), v.z);
}

void World::removeFarChunks(glm::vec3 block_position_to_render_around){
    int chunks_removed_this_frame = 0;


    glm::ivec3 chunk_position_to_render_around = getChunkCoordsFromBlock(block_position_to_render_around);
    

    for (auto chunk_pair = worldChunks.cbegin(); chunk_pair != worldChunks.cend(); ) {
        // Careful not to delete a chunk that is in the middle of being generated
        if( glm::compMax( glm::abs(chunk_pair->first - chunk_position_to_render_around)) > radius + 2 && chunk_pair->second->terrainPopulationFinished ){
            SimpleTimer timer;

            
            // printf("Removing chunk with chunk index:  %d %d %d  \n", chunk_pair->first.x,chunk_pair->first.y,chunk_pair->first.z);
            // Will this clean up the chunk object?
            delete worldChunks[chunk_pair->first];
            worldChunks.erase(chunk_pair++);

            chunks_removed_this_frame++;
            interface->chunkDeletions++;


            interface->chunkDelTime->enqueue(timer.end());

            if(chunks_removed_this_frame >= 1){
                return;
            }
        }else{
            ++chunk_pair;
        }
    }


}

void* World::on_thread_gen_mesh(void){

    while(true){

        // Wait until main thread enqueues a chunk
        std::unique_lock<std::mutex> lk(chunk_mesh_gen_m);
        chunk_mesh_gen_cv.wait(lk, [&]{return !chunks_for_mesh_gen.empty();});

        Chunk* chunk = chunks_for_mesh_gen.front();
        chunks_for_mesh_gen.pop();
        lk.unlock();

        // Build the mesh
        SimpleTimer timer;
        chunk->generateGreedyMesh();
        interface->chunkMeshGenTime->enqueue(timer.end());
        chunk->mesh->isFullyGenerated = true;
    }
    std::cout<<"Error: chunk meshing thread exiting"<<std::endl;
    pthread_exit(0);

}

void* World::on_thread_chunk_init(void){

    while(true){
        
        // Wait until a chunk to init is enqueued
        std::unique_lock<std::mutex> lk(chunks_for_init_m);
        chunks_for_init_cv.wait(lk, [&]{return !chunks_for_init.empty();});

        // Get the chunk and unlock
        Chunk* chunk = chunks_for_init.front();
        chunks_for_init.pop();
        lk.unlock();

        //Initialise the chunk, which is just do the terrain for now.
        chunk->initialiseTerrain();
    }
    std::cout<<"Error: chunk initialising thread exiting"<<std::endl;
    pthread_exit(0);

}

void World::addNewChunks(glm::vec3 block_position_to_render_around){

    int chunks_added_this_frame = 0;

    glm::ivec3 chunk_position_to_render_around = getChunkCoordsFromBlock(block_position_to_render_around);
    glm::ivec3 chunk_offset = glm::ivec3(0);


    for(chunk_offset.x = -radius; chunk_offset.x < radius+1; chunk_offset.x++){
    for(chunk_offset.y = -radius; chunk_offset.y < radius+1; chunk_offset.y++){
    for(chunk_offset.z = -radius; chunk_offset.z < radius+1; chunk_offset.z++){
        
        glm::ivec3 chunk_to_add_pos = chunk_position_to_render_around + chunk_offset;

        if(worldChunks.find(chunk_to_add_pos) == worldChunks.end()){

            SimpleTimer timer;

            chunks_added_this_frame++;
            interface->chunkInitialisations++;

            Chunk* new_chunk = new Chunk(this, interface ,chunk_to_add_pos.x,chunk_to_add_pos.y,chunk_to_add_pos.z, &terrain, renderer);
            worldChunks[chunk_to_add_pos] = new_chunk;
            // printf("Adding chunk with chunk index:  %d %d %d  \n", chunk_to_add_pos.x,chunk_to_add_pos.y,chunk_to_add_pos.z);

            interface->chunkInitTime->enqueue(timer.end());

            if(chunks_added_this_frame >= 1){
                return;
            }


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

    // frame_counter++;

    // if (frame_counter % 100 == 0){
    //     printf("Chunks for init queue length: %d \n", chunks_for_init.size());
    //     printf("Chunks for meshing queue length: %d \n", );
    // }

    interface->chunk_mesh_gen_queue_size = chunks_for_mesh_gen.size();
    interface->chunk_init_queue_size = chunks_for_init.size();
    
    cleanStoredChunks(camera.Position / CAMERA_TO_WORLD_POS_SCALE);

    for (auto& chunk_pair : worldChunks){
        chunk_pair.second->renderChunk(camera);
    }


}