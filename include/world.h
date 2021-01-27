#ifndef WORLD_H
#define WORLD_H

// #include "chunk.h"
#include "view.h"
#include "camera.h"

class Chunk;

class World{
public:
    std::vector<Chunk*> worldChunks;
    
    World();
    void render(Renderer &renderer, Camera &camera);
    
};

#endif