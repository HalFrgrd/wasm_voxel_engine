#ifndef VIEW_H
#define VIEW_H

#include <SDL.h>

#include <SDL_opengles2.h>

#include "shader.h"
#include "camera.h"


int add(int x, int y);

float getBackgroundColor();

class My_renderer{
public:
    const int screen_width = 800;
    const int screen_height = 600;


    SDL_Window *window;
    GLuint vbo, vao;
    Shader my_shader;


    void initRender();


};

#endif