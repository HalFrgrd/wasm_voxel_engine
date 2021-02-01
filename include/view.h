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
    const int screen_width = 800;
    const int screen_height = 600;


    SDL_Window *window;
    GLuint vbo, vao, colours;
    Shader my_shader;


    void initRender();
    GLuint getVertexBuffer();
    GLuint getColourBuffer();



};

#endif