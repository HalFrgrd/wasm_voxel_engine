

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

    World world(&renderer);

    EventHandler events;
    SDL_Event eventHolder;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    int lastTime = SDL_GetTicks();
    int currentTime;
    int numFrames = 0;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(renderer.window, renderer.context);
    // ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui_ImplOpenGL3_Init();
    
    loop = [&] {
        events.process(&eventHolder, camera);
		events.update(camera);

        ImGuiIO& io = ImGui::GetIO();
        // IM_UNUSED(arg); // We can pass this argument as the second parameter of emscripten_set_main_loop_arg(), but we don't use that.

        // Our state (make them static = more or less global) as a convenience to keep the example terse.
        bool show_demo_window = true;
        bool show_another_window = false;
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.50f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(renderer.window);
        ImGui::NewFrame();
        

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                                // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");                     // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);            // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                  // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color);       // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                                  // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // currentTime = SDL_GetTicks();
        // numFrames++;
        // if(currentTime - lastTime >= 1000){
        //     std::cout << (float) numFrames * 1000.0f / (float) (currentTime - lastTime) << std::endl;
        //     numFrames = 0;
        //     lastTime = currentTime;
        // }


        // Rendering
        SDL_GL_MakeCurrent(renderer.window, renderer.context);
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
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
