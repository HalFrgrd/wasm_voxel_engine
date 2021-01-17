#include "chunk.h"


void Chunk::renderChunk(My_renderer &renderer, Camera &camera){


    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, abs(sin(SDL_GetTicks()/100.0)), 0.5));  

    // glm::mat4 model_matrix = glm::mat4(0.1f);
    // renderer.my_shader.setMat4("model", model_matrix);

    renderer.my_shader.setMat4("view", camera.GetViewMatrix());

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)renderer.screen_width / (float)renderer.screen_width, 0.1f, 100.0f);
    renderer.my_shader.setMat4("projection", projection);


    // Clear the screen
    if( background_is_black )
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindVertexArray(renderer.vao);
    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        renderer.my_shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    // move a vertex
    // const uint32_t milliseconds_since_start = SDL_GetTicks();
    // const uint32_t milliseconds_per_loop = 3000;
    // vertices[0] = ( milliseconds_since_start % milliseconds_per_loop ) / float(milliseconds_per_loop) - 0.5f;
	
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ibo_cube_elements);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ibo_cube_elements);
    // int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    // glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


    SDL_GL_SwapWindow(renderer.window);
}