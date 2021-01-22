#include "world.h"


World::World(){
    

    for (int i = 0; i < 16; i++)
    {
        worldChunks[i].setChunkCoords(i%4,0,i/4);
    }
    
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



    for (int i = 0; i < 1; i++)
    {
        worldChunks[i].renderChunk(renderer, camera);
    }

    SDL_GL_SwapWindow(renderer.window);

}