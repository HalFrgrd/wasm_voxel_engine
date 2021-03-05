#ifndef WORLD_H
#define WORLD_H

#define GL_GLEXT_PROTOTYPES 1

#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>

// #include "chunk.h"
#include "view.h"
#include "camera.h"
#include "block.h"
#include "terrain.h"
#include <glm/gtx/hash.hpp>
#include <glm/gtx/component_wise.hpp>
#include "interface.h"
#include "timer.h"

#include <unordered_map>
#include "pthread.h"
#include <queue>
#include <mutex>
#include <condition_variable>


// Forward declaration of chunk
class Chunk;

class World{
public:
    World(Renderer *initRenderer, GUI_Interface *initInterface);
    std::unordered_map<glm::ivec3, Chunk*> worldChunks;
    TerrainGenerator terrain;
    Renderer *renderer;
    GUI_Interface *interface;

    void render(Camera &camera);
    glm::vec3 blockPosToRenderAround; // center of rendering
    glm::ivec3 chunk_position_to_render_around;
    bool shouldRenderChunk(Chunk *chunk);



    // Chunk renderering
    const static int radius = 3;
    void removeFarChunks();
    void addNewChunks();
    void cleanStoredChunks();

    Block::BlockType worldgetBlockFromWorld(glm::ivec3 blockPos);

    Chunk* getChunk(int chunkX, int chunkY, int chunkZ);

    // The world class also acts as the chunk manager class
    // Asynchronous chunk meshing
    void *on_thread_gen_mesh(void);
    static void *on_thread_gen_mesh_helper(void *context)
    {
        return ((World *)context)->on_thread_gen_mesh();
    }
    pthread_t chunk_mesh_gen_thread;
    std::queue<Chunk*> chunks_for_mesh_gen;
    std::mutex chunk_mesh_gen_m;
    std::condition_variable chunk_mesh_gen_cv;

    // Asynchronous chunk initialisation
    void *on_thread_chunk_init(void);
    static void *on_thread_chunk_init_helper(void *context)
    {
        return ((World *)context)->on_thread_chunk_init();
    }
    pthread_t chunk_init_thread;
    std::queue<Chunk*> chunks_for_init;
    std::mutex chunks_for_init_m;
    std::condition_variable chunks_for_init_cv;
    
};

#endif