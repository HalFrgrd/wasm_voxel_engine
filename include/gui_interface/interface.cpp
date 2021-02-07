#include "interface.h"


GUI_Interface::GUI_Interface(Renderer *initRenderer): renderer(initRenderer) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = & ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    // io.IniFilename = NULL;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(renderer->window, renderer->context);
    // ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui_ImplOpenGL3_Init();


    f = 0.0f;
    counter = 0;
}

void GUI_Interface::update(){


    // Our state (make them static = more or less global) as a convenience to keep the example terse.
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.50f);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(renderer->window);
    ImGui::NewFrame();

     
    

    // For some reason the demo window needs to be rendered once for something to show
    if (show_demo_window){
        bool test = true;
        ImGui::ShowDemoWindow(&test);
        show_demo_window = false;
    }

    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

    

    ImGui::Begin("Debug info"); 

    ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if( ImGui::TreeNode("Chunk rendering")){
        ImGui::Text("Chunks rendered: %d", counter);
        ImGui::TreePop();
    }



    ImGui::End();


}