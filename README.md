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
1. Install emcc
2. Change the cmake compiler
