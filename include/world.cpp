#include "world.h"


void World::initWorld(){
    Chunk test_chunk;
    worldChunks[0] = test_chunk;
}

void World::render(Renderer &renderer, Camera &camera){
    

    worldChunks[0].renderChunk(renderer, camera);
}