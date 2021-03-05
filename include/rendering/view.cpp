

#define GL_GLEXT_PROTOTYPES 1

#include <iostream>
#include "view.h"

GLuint Renderer::createVertexArrayBuffer(){
  GLuint vao;
     
  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);

  return vao;
}

GLuint Renderer::createVertexBuffer(){
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

  GLint posAttrib = my_shader.getAttribLocation("position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return buffer_id;
}

GLuint Renderer::createIndexBuffer(){
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
  return buffer_id;
}


GLuint Renderer::createColourBuffer(){
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

  GLint colourAttrib = my_shader.getAttribLocation("vertex_colour");
  glEnableVertexAttribArray(colourAttrib);
  glVertexAttribPointer(colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return buffer_id;
}

void Renderer::initRender(){

  screen_width = 1400;
  screen_height = 720;

  // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }


    // const char* glsl_version = "#version 100";
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Voxel Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, window_flags);
    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return;
    }

    // The below says why we shouldn't use setswapinterval.
    //https://tristanpenman.com/blog/posts/2018/01/08/porting-an-asteroids-clone-to-javascript/
    //SDL_GL_SetSwapInterval(1); // Enable vsync

    
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // glFrontFace(GL_CCW);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);

    // Initialise shaders
    my_shader.initShader("./include/shaders/vertex_shader.vs","./include/shaders/fragment_shader.fs");
    my_shader.activate();

    
}
