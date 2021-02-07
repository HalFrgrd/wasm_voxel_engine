cmake_minimum_required(VERSION 3.16.3)

# use emcc instead of your default cpp compiler
set(CMAKE_CXX_COMPILER "emcc")

#project name
project(wasm_voxel_engine)


set(CMAKE_CXX_STANDARD 11)

# specific flags for the emcc command
# we need to embed (or preload) our assets
# set(CMAKE_CXX_FLAGS  
#     "-O1 -s WASM=1 -s USE_SDL=2 -s NO_FILESYSTEM=1 -DIMGUI_DISABLE_FILE_FUNCTIONS -s TOTAL_MEMORY=1024MB -s ALLOW_MEMORY_GROWTH=1 -s LLD_REPORT_UNDEFINED --embed-file ../include/shaders"
# )

# Probably a better way to do this
set(CMAKE_CXX_FLAGS  
    "-O1 -s WASM=1 --embed-file ../include/shaders -s USE_SDL=2 -s TOTAL_MEMORY=1024MB -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s LLD_REPORT_UNDEFINED"
)

include_directories(include)
include_directories(include/gui_interface/imgui)
include_directories(include/gui_interface)
include_directories(include/physical_world)
include_directories(include/rendering)
include_directories(include/helper)

file(GLOB SOURCES  
    "include/*.cpp"
    "include/gui_interface/imgui/*.cpp" 
    "include/gui_interface/*.cpp" 
    "include/physical_world/*.cpp" 
    "include/rendering/*.cpp" 
    "include/helper/*.cpp" 
    )

add_executable(wasm_voxel_engine main.cpp ${SOURCES})
set_target_properties(wasm_voxel_engine PROPERTIES OUTPUT_NAME "index")
