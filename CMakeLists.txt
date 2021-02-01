cmake_minimum_required(VERSION 3.16.3)

# use emcc instead of your default cpp compiler
set(CMAKE_CXX_COMPILER "emcc")

#project name
project(wasm_voxel_engine)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

# specific flags for the emcc command
# we need to embed (or preload) our assets
set(CMAKE_CXX_FLAGS  
    "-O1 -s WASM=1 -s USE_SDL=2 -s TOTAL_MEMORY=1024MB -s LLD_REPORT_UNDEFINED --embed-file ../include/shaders"
)

file(GLOB SOURCES "include/*.cpp")

add_executable(wasm_voxel_engine main.cpp ${SOURCES})
set_target_properties(wasm_voxel_engine PROPERTIES OUTPUT_NAME "index")
