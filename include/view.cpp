

#define GL_GLEXT_PROTOTYPES 1

#include <iostream>
#include "view.h"

GLuint Renderer::getVertexArrayBuffer(){
  GLuint vao;
     
  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);

  return vao;
}

GLuint Renderer::getVertexBuffer(){
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

  GLint posAttrib = my_shader.getAttribLocation("position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return buffer_id;
}

GLuint Renderer::getColourBuffer(){
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

  GLint colourAttrib = my_shader.getAttribLocation("vertex_colour");
  glEnableVertexAttribArray(colourAttrib);
  glVertexAttribPointer(colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return buffer_id;
}

void Renderer::initRender(){

    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
      // glFrontFace(GL_CCW);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);

 

    // glGenBuffers(1, &colours);
    // glGenBuffers(1, &vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    my_shader.initShader("./include/shaders/vertex_shader.vs","./include/shaders/fragment_shader.fs");
    my_shader.activate();

    // Specify the layout of the vertex data
    // GLint posAttrib = my_shader.getAttribLocation("position");
    // glEnableVertexAttribArray(posAttrib);
    // glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // glBindBuffer(GL_ARRAY_BUFFER, colours);
    // GLint colourAttrib = my_shader.getAttribLocation("vertex_colour");
    // glEnableVertexAttribArray(colourAttrib);
    // glVertexAttribPointer(colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
}
