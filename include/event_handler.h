#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL.h>
#include "camera.h"

class EventHandler{
public:
    void process(SDL_Event *e, Camera &camera);
    void update(Camera &camera);
    int prev_x;
    int prev_y;
    bool right_click_depressed;

    bool w_depressed;
    bool a_depressed;
    bool s_depressed;
    bool d_depressed;
    bool shift_depressed;
    bool space_depressed;


    float prev_y_scroll; 
};


#endif