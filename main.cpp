



#include <stdio.h>
#include <functional>
#include <iostream>

#include <emscripten.h>
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#include "view.h"
#include "event_handler.h"
#include "camera.h"
#include "world.h"
#include "interface.h"

// Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
// Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
// SDL_Window*     g_Window = NULL;
// SDL_GLContext   g_GLContext = NULL;

std::function<void()> loop;
// Emscripten / browser will limit main_loop to 60 Hz.
// Easy hack to boost frame rate, just rain the loop multple
// times per call of main_loop! 
void main_loop() { loop(); loop(); loop(); loop(); }

int main()
{
    Renderer renderer;
    renderer.initRender();

    GUI_Interface interface(&renderer);

    World world(&renderer, &interface);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    EventHandler events(&camera, &interface);
    SDL_Event eventHolder;


    int lastTime = SDL_GetTicks();
    int currentTime;
    int numFrames = 0;
    
    loop = [&] {
        events.process(&eventHolder);
		events.update();

        interface.update();

        // Rendering
        SDL_GL_MakeCurrent(renderer.window, renderer.context);
        glViewport(0, 0, (int)interface.io->DisplaySize.x, (int)interface.io->DisplaySize.y);
        
        glClearColor(144.0/255.0, 202.0/255.0, 249.0/255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui::Render();
        world.render(camera);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(renderer.window);
    };

    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop(main_loop, 0, true);

    

    return EXIT_SUCCESS;

}
