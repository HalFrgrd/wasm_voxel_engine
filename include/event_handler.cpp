#include "event_handler.h"

void EventHandler::process(SDL_Event *e, Camera &camera){


    while(SDL_PollEvent(e) != 0){
        if(e->type == SDL_KEYDOWN){
            switch(e->key.keysym.sym){
                case SDLK_w:
                    w_depressed = true;
                    break;
                case SDLK_a:
                    a_depressed = true;
                    break;
                case SDLK_s:
                    s_depressed = true;
                    break;
                case SDLK_d:
                    d_depressed = true;
                    break;
                case SDLK_LSHIFT:
                    shift_depressed = true;
                    break;
                case SDLK_SPACE:
                    space_depressed = true;
                    break;
            }
        } else if(e->type == SDL_KEYUP){
            switch(e->key.keysym.sym){
                case SDLK_w:
                    w_depressed = false;
                    break;
                case SDLK_a:
                    a_depressed = false;
                    break;
                case SDLK_s:
                    s_depressed = false;
                    break;
                case SDLK_d:
                    d_depressed = false;
                    break;
                case SDLK_LSHIFT:
                    shift_depressed = false;
                    break;
                case SDLK_SPACE:
                    space_depressed = false;
                    break;
            }
        }
        else if(e->type == SDL_MOUSEMOTION){
            int x, y;
            SDL_GetMouseState(&x, &y);
            camera.ProcessMouseMovement(x - prev_x, prev_y - y, right_click_depressed);
            prev_x = x;
            prev_y = y;
        }
        else if (e->type == SDL_MOUSEWHEEL){
            // float new_y_scroll = e->wheel.y;
            camera.ProcessMouseScroll(e->wheel.y);
            // prev_y_scroll = new_y_scroll;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT){
            right_click_depressed = true;
        }
        else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_RIGHT){
            right_click_depressed = false;
        }
    }
}


void EventHandler::update(Camera &camera){
    if(w_depressed)  camera.ProcessKeyboard(FORWARD, 0.05f);
    if(a_depressed)  camera.ProcessKeyboard(LEFT, 0.05f);
    if(s_depressed)  camera.ProcessKeyboard(BACKWARD, 0.05f);
    if(d_depressed)  camera.ProcessKeyboard(RIGHT, 0.05f);
    if(shift_depressed)  camera.ProcessKeyboard(DOWNWARD, 0.05f);
    if(space_depressed)  camera.ProcessKeyboard(UPWARD, 0.05f);
   

}