#ifndef WORLD_H
#define WORLD_H

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
    std::unordered_map<glm::ivec3, Chunk*> worldChunks;
    TerrainGenerator terrain;
    
    World(Renderer *initRenderer, GUI_Interface *initInterface);
    Renderer *renderer;
    GUI_Interface *interface;

    // Count how many frames there have been, used for debugging
    // long frame_counter = 0;

    void render(Camera &camera);

    // Chunk renderering
    const static int radius = 3;
    void removeFarChunks(glm::vec3 block_position_to_render_around);
    void addNewChunks(glm::vec3 block_position_to_render_around);
    void cleanStoredChunks(glm::vec3 block_position_to_render_around);

    Block::BlockType worldgetBlockFromWorld(int blockX, int blockY, int blockZ);

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