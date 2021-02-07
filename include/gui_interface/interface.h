#ifndef INTERFACE_H
#define INTERFACE_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "view.h"

class GUI_Interface {
public:
    Renderer *renderer;
    ImGuiIO *io;

    float f ;
    int counter ;

    GUI_Interface(Renderer *initRenderer);

    void update();
};


#endif