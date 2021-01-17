#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "view.h"
#include "camera.h"

class World{
public:
    Chunk worldChunks[10];
    void initWorld();
    void render(Renderer &renderer, Camera &camera);
    
};

#endif