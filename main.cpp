#include <functional>
#include <iostream>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1

#include "view.h"
#include "event_handler.h"
#include "camera.h"
#include "world.h"


// // an example of something we will control from the javascript side
// bool background_is_black = true;

// // the function called by the javascript code
// extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{

    Renderer renderer;
    renderer.initRender();

    World world(&renderer);

    EventHandler events;
    SDL_Event eventHolder;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    int lastTime = SDL_GetTicks();
    int currentTime;
    int numFrames = 0;

    loop = [&]
    {
        events.process(&eventHolder, camera);
		events.update(camera);

        world.render( camera);

        currentTime = SDL_GetTicks();
        numFrames++;
        if(currentTime - lastTime >= 1000){
            std::cout << (float) numFrames * 1000.0f / (float) (currentTime - lastTime) << std::endl;
            numFrames = 0;
            lastTime = currentTime;
        }
    };

    // https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-5-move-owl/
    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
