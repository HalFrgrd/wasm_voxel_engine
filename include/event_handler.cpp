#include "event_handler.h"

void EventHandler::update(SDL_Event *e, Camera &camera){


    while(SDL_PollEvent(e) != 0){
        if(e->type == SDL_KEYDOWN){
            switch(e->key.keysym.sym){
                case SDLK_w:
                    camera.ProcessKeyboard(FORWARD, 0.05f);
                    break;
                case SDLK_a:
                    camera.ProcessKeyboard(LEFT, 0.05f);
                    break;
                case SDLK_s:
                    camera.ProcessKeyboard(BACKWARD, 0.05f);
                    break;
                case SDLK_d:
                    camera.ProcessKeyboard(RIGHT, 0.05f);
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
