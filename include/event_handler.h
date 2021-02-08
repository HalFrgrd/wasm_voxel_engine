#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL.h>
#include "camera.h"
#include "interface.h"

class EventHandler{
public:
    EventHandler(Camera *initCamera, GUI_Interface *initInterface); 
    Camera *camera;
    GUI_Interface *interface;


    void process(SDL_Event *e);
    void update();
    int prev_x;
    int prev_y;
    bool right_click_depressed = false;

    bool w_depressed = false;
    bool a_depressed = false;
    bool s_depressed = false;
    bool d_depressed = false;
    bool shift_depressed = false;
    bool space_depressed = false;


    float prev_y_scroll; 
};


#endif 