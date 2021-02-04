

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

  // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    // For the browser using Emscripten, we are going to use WebGL1 with GL ES2. See the Makefile. for requirement details.
    // It is very likely the generated file won't work in many browsers. Firefox is the only sure bet, but I have successfully
    // run this code on Chrome for Android for example.
    const char* glsl_version = "#version 100";
    //const char* glsl_version = "#version 300 es";
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
    window = SDL_CreateWindow("Dear ImGui Emscripten example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return;
    }
    SDL_GL_SetSwapInterval(1); // Enable vsync

    

    // context = SDL_GL_CreateContext(window);
    // SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, nullptr);

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // SDL_GL_SetSwapInterval(1);

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
