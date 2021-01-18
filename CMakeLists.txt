cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_COMPILER "/home/hal/emsdk/upstream/emscripten/emcc")
project(wasm_voxel_engine)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS  
    "-O1 -s WASM=1 -s USE_SDL=2 -s LLD_REPORT_UNDEFINED --embed-file ../include/shaders"
)

set(SOURCES main.cpp include/view.cpp include/chunk.cpp include/shader.cpp include/event_handler.cpp include/camera.cpp include/world.cpp)

add_executable(wasm_voxel_engine ${SOURCES})
set_target_properties(wasm_voxel_engine PROPERTIES OUTPUT_NAME "index")
