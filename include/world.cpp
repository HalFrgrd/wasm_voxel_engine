#include "world.h"


World::World(){
    

    for (int i = 0; i < 16; i++)
    {
        worldChunks[i].setChunkCoords(i%4,0,i/4);
    }
    
}

void World::render(Renderer &renderer, Camera &camera){
    for (int i = 0; i < 1; i++)
    {
        worldChunks[i].renderChunk(renderer, camera);
    }
}