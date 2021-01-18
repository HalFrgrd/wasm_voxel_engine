#include "chunk.h"


int Chunk::flattenCoords(int i, int j, int k){
    return i + chunkSize*j + chunkSize*chunkSize*k;
}

glm::vec3 Chunk::unflattenCoords(int i){
    return glm::vec3(i% (chunkSize), (i/chunkSize)%chunkSize, (i/(chunkSize*chunkSize))%chunkSize  );
}

void Chunk::setChunkCoords(int x, int y, int z){
    chunkX = x;
    chunkY = y;
    chunkZ = z;

}

Chunk::Chunk(){

        for(int k = 0; k < chunkSize; k++){
            for(int i = 0; i < chunkSize; i++){
        for(int j = 0; j < chunkSize; j++){
                int heightThreshold = 4+ (rand()%4);
                if(j > heightThreshold){
                    cubePositions[flattenCoords(i,j,k)] = Block::BLOCK_AIR;
                } else{
                    cubePositions[flattenCoords(i,j,k)] = Block::BLOCK_DIRT;
                }
            }
        }
    }
}


void Chunk::renderChunk(Renderer &renderer, Camera &camera){

    renderer.my_shader.setMat4("view", camera.GetViewMatrix());

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)renderer.screen_width / (float)renderer.screen_height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::mat4(1.0f);
    renderer.my_shader.setMat4("projection", projection);

    // Set background color
    glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Block::vertices), Block::vertices, GL_STATIC_DRAW);

    // glBindVertexArray(renderer.vao);
    glm::vec3 chunkShift = glm::vec3(chunkX * chunkSize, chunkY * chunkSize,chunkZ * chunkSize );

    // std::cout<< chunkShift.x << chunkShift.y << chunkShift.z << std::endl;


    for (unsigned int i = 0; i < chunkSize*chunkSize*chunkSize; i++)
    {   
        if(cubePositions[i] != Block::BLOCK_AIR){

            // calculate the model matrix for each object and pass it to shader before drawing

            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::translate(model, unflattenCoords(i));
            model = glm::translate(model, chunkShift);
            renderer.my_shader.setMat4("model", model);
            renderer.my_shader.setVec3("main_colour", glm::vec3(0.7,0.5,0.1));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            renderer.my_shader.setVec3("main_colour", glm::vec3(0.0,0.0,0.0));
            glDrawArrays(GL_LINE_STRIP, 0, 36);
        }
    }

    SDL_GL_SwapWindow(renderer.window);
}