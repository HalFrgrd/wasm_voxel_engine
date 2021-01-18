#include "chunk.h"


int Chunk::flattenCoords(int i, int j, int k){
    return i + chunkSize*j + chunkSize*chunkSize*k;
}

Chunk::Chunk(){
    for(int i = 0; i < chunkSize; i++){
        for(int j = 0; j < chunkSize; j++){
            for(int k = 0; k < chunkSize; k++){
                cubePositions[flattenCoords(i,j,k)] = glm::vec3(i,j,k);
            }
        }
    }
}


void Chunk::renderChunk(Renderer &renderer, Camera &camera){


    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, abs(sin(SDL_GetTicks()/100.0)), 0.5));  

    // glm::mat4 model_matrix = glm::mat4(0.1f);
    // renderer.my_shader.setMat4("model", model_matrix);

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
    for (unsigned int i = 0; i < chunkSize*chunkSize*chunkSize; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        // float angle = 20.0f * i;
        // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        renderer.my_shader.setMat4("model", model);

        glDrawArrays(GL_LINE_LOOP, 0, 36);
    }

    SDL_GL_SwapWindow(renderer.window);
}