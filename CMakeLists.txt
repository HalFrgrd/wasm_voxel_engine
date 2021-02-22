cmake_minimum_required(VERSION 3.16.3)

# use emcc instead of your default cpp compiler
set(CMAKE_CXX_COMPILER "emcc")

#project name
project(wasm_voxel_engine)

set(CMAKE_CXX_STANDARD 11)

# specific flags for the emcc command
# we need to embed (or preload) our assets
set(CMAKE_CXX_FLAGS  
    "-O2 \
    --embed-file ../include/shaders \
    -pthread \
    --profiling \
    -s WASM=1 \
    -s USE_SDL=2 \
    -s TOTAL_MEMORY=1024MB  \
    -s DISABLE_EXCEPTION_CATCHING=1 \
    -s NO_EXIT_RUNTIME=0     \
    -s ASSERTIONS=1     \
    -s LLD_REPORT_UNDEFINED  \
    -s DISABLE_EXCEPTION_CATCHING=0  "
)
#-s ALLOW_MEMORY_GROWTH this doesnt work well with pthreads

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
