# wasm_voxel_engine
 Voxel engine for wasm
Chunks:
Stored using run length encoding, some kind of balanced bin tree.

Mesh cache: Store visible surfaces. 
This isn't the final mesh. 
But it allows for modifying the mesh without having rebuild the whole mesh.
If we went from the chunk bin tree to the greedy mesh, placing a block in the chunk would mean having to redraw the mesh from scratch.
Hopefully most chunks will have very few visible surfaces so that chunk size is kept small during gameplay.

Visible faces are stored 



Drawing a face requires 4 vertices;
A B C
  B C D
Can we use this significant overlap? i.e. striding one?


## Installing
1. Install emsdk.
2. In CMakeLists.txt, change the path to your emcc compiler.
3. Create a folder called `build`.
4. Cd into the `build` folder.
5. Run `emcmake cmake ..` .
6. Run `emmake make VERBOSE=1`.
7. You need to host a web server from the root dir. Easily done with python `python -m http.server 8080`.

If you edit the shaders, make won't detect changes, so make sure you edit a cpp file also.


## Notes
emcc -I./include main.cpp include/view.cpp include/chunk.cpp include/shader.cpp include/event_handler.cpp include/camera.cpp include/world.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -O1 -s LLD_REPORT_UNDEFINED --preload-file include/shaders -o index.js
