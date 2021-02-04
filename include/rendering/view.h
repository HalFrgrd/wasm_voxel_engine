#ifndef VIEW_H
#define VIEW_H

#include <SDL.h>

#include <SDL_opengles2.h>

#include "shader.h"
#include "camera.h"


int add(int x, int y);

float getBackgroundColor();

class Renderer{
public:
    int screen_width;
    int screen_height;


    SDL_Window *window;
    SDL_GLContext context;

    GLuint vertex_array_object;

    // GLuint vbo, vao, colours;
    Shader my_shader;


    void initRender();
    GLuint getVertexArrayBuffer();
    GLuint getVertexBuffer();
    GLuint getColourBuffer();


};

#endif