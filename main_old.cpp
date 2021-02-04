#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

// // an example of something we will control from the javascript side
// bool background_is_black = true;

// // the function called by the javascript code
// extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

std::function<void()> loop;
// Emscripten / browser will limit main_loop to 60 Hz.
// Easy hack to boost frame rate, just rain the loop multple
// times per call of main_loop! 
void main_loop() { loop(); loop(); loop(); loop(); }

int main()
{

    Renderer renderer;
    renderer.initRender();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    // window is the SDL_Window*
    // context is the SDL_GLContext
    ImGui_ImplSDL2_InitForOpenGL(renderer.window, renderer.context);
    ImGui_ImplOpenGL3_Init();



    World world(&renderer);

    EventHandler events;
    SDL_Event eventHolder;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    int lastTime = SDL_GetTicks();
    int currentTime;
    int numFrames = 0;

    float speed = 0.0f;
    bool someBoolean = true;

    loop = [&]
    {
        events.process(&eventHolder, camera);
		events.update(camera);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(renderer.window);
        ImGui::NewFrame();

        ImGui::Begin("MyWindow");
        ImGui::Checkbox("Boolean property", &someBoolean);
        if(ImGui::Button("Reset Speed")) {
            // This code is executed when the user clicks the button
            speed = 0;
        }
        ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
        ImGui::End();

        // Set background color
        glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
        //https://learnopengl.com/Getting-started/Coordinate-Systems read the bit about depth
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.render(camera);

        ImGui::Render();
        SDL_GL_MakeCurrent(renderer.window, renderer.context);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        SDL_GL_SwapWindow(renderer.window);


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
