#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL.h>
#include "camera.h"

class EventHandler{
public:
    void update(SDL_Event *e, Camera &camera);
    int prev_x;
    int prev_y;
    bool right_click_depressed;
    float prev_y_scroll; 
};


#endif