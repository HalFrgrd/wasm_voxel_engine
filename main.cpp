#include <functional>
#include <iostream>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1

#include "view.h"
#include "chunk.h"
#include "event_handler.h"
#include "camera.h"


// // an example of something we will control from the javascript side
// bool background_is_black = true;

// // the function called by the javascript code
// extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{

    My_renderer my_rend;

    Chunk my_chunk;

    my_rend.initRender();

    EventHandler events;

    SDL_Event e;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    loop = [&]
    {
		events.update(&e, my_chunk, camera);

        my_chunk.renderChunk(my_rend, camera);
    };

    // https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-5-move-owl/
    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
