#include "chunk.h"

inline int Chunk::flattenCoords(glm::ivec3 coords){
    return coords.x + chunkSize*coords.y + chunkSize*chunkSize*coords.z;
};


inline glm::ivec3 Chunk::unflattenCoords(int i){
    return glm::ivec3(i% (chunkSize), (i/chunkSize)%chunkSize, (i/(chunkSize*chunkSize))%chunkSize  );
}

inline glm::ivec3 Chunk::localPosToGlobalPos(glm::ivec3 localBlockPos){
    return chunkPos * chunkSize + localBlockPos;
}

inline Block::BlockType Chunk::getBlockFromChunk(glm::ivec3 coords){
    return cubePositions[flattenCoords(coords)]; 
}



inline  Block::BlockType Chunk::getBlockFromWorld(glm::ivec3 coords){

    if(coords.x >=0 && coords.x < chunkSize &&
        coords.y >=0 && coords.y < chunkSize &&
        coords.z >=0 && coords.z < chunkSize )
        {
            return getBlockFromChunk(coords);
        }
    return my_world->worldgetBlockFromWorld( localPosToGlobalPos(coords) );
};


Chunk::Chunk(World *initWorld, GUI_Interface *initInterface, glm::ivec3 initChunkPos, TerrainGenerator* initTerrain, Renderer *initRenderer) {
    
    mesh = new ChunkMesh(initRenderer);

    my_world = initWorld;
    interface = initInterface;
    terrain = initTerrain;

    chunkPos = initChunkPos;

    // Do terrain initialisation on another thread
    doNotDelete = true;
    {
        std::lock_guard<std::mutex> lk(my_world->chunks_for_init_m);
        my_world->chunks_for_init.push(this);
    }
    my_world->chunks_for_init_cv.notify_one();


}

void Chunk::initialiseTerrain() {
    int block_coords = 0;
    for(int i = 0; i < chunkSize; i++){
        for(int j = 0; j < chunkSize; j++){
            for(int k = 0; k < chunkSize; k++){
                block_coords = flattenCoords(glm::ivec3(i,j,k));
                cubePositions[block_coords] = terrain->getBlock(localPosToGlobalPos( glm::ivec3(i,j,k)));
                isEmpty &= (cubePositions[block_coords] == Block::BLOCK_AIR);
            }
        }
    }

    terrainPopulationFinished = true;
}

Chunk::~Chunk(){
    delete mesh;
    delete [] cubePositions;
}

bool Chunk::isBlockFaceVisible(glm::ivec3 blockPos, int axis, bool isBackFace){
    glm::ivec3 temp = blockPos;
    if(isBackFace){
        temp[axis] -=1;
    } else{
        temp[axis] += 1;
    }

    return !Block::isSolid(getBlockFromWorld(temp));
}

bool Chunk::compareStep(glm::ivec3 a, glm::ivec3 b, int direction, bool isBackFace){
    Block::BlockType blockA = getBlockFromWorld(a);
    Block::BlockType blockB = getBlockFromWorld(b);

    return blockA == blockB && Block::isSolid(blockA) && isBlockFaceVisible(b, direction, isBackFace);
}

void Chunk::generateGreedyMesh(){

    if(isEmpty){
        return;
    }

    // https://eddieabbondanz.io/post/voxel/greedy-mesh/

    glm::vec3 chunkShift = chunkPos * chunkSize;
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, chunkShift);
    
    mesh->vertices.clear();
    mesh->colours.clear();

    glm::ivec3 quadSize;

    glm::vec4 m; 
    glm::vec4 n;
    glm::vec4 offsetPos;

    glm::ivec3 startPos;
    glm::ivec3 currPos;

    glm::vec3 quadVertices[4];

    int direction, workAxis1, workAxis2;

    bool merged[chunkSize][chunkSize];

    Block::BlockType startBlock; 

    for (int face = 0; face < 6; face++){
        bool isBackFace = face > 2;

        glm::vec3 quadColour;

        if(face == 0){ 
            quadColour = glm::vec3(1.0f,0.0f,0.0f);
        }
        if(face == 1){
            quadColour = glm::vec3(0.0f,1.0f,0.0f);
        }
        if(face == 2){
            quadColour = glm::vec3(0.0f,0.0f,1.0f);
        }
        if(face == 3){ 
            quadColour = glm::vec3(1.0f,1.0f,1.0f);
        }
        if(face == 4){
            quadColour = glm::vec3(1.0f,1.0f,0.0f);
        }
        if(face == 5){
            quadColour = glm::vec3(0.5f,0.5f,0.5f);
        }



        direction = face%3;
        workAxis1 = (direction + 1)%3;
        workAxis2 = (direction + 2)%3;

        startPos = glm::ivec3(0);
        currPos = glm::ivec3(0);

        for(startPos[direction] = 0; startPos[direction] < chunkSize; startPos[direction]++){
            memset(merged, false, sizeof(merged) ); //reset to false

            for (startPos[workAxis1] = 0; startPos[workAxis1] < chunkSize; startPos[workAxis1]++) {
                for (startPos[workAxis2] = 0; startPos[workAxis2] < chunkSize; startPos[workAxis2]++) {
                    

                    startBlock = getBlockFromWorld(startPos);

                    // If this face is already merged or the block is not solid or the face is not visible
                    if(
                        merged[startPos[workAxis1]][startPos[workAxis2]] || 
                        !Block::isSolid(startBlock) ||  
                        !isBlockFaceVisible(startPos,direction,isBackFace)){
                            continue;
                        }
                            

                    
                    quadSize = glm::ivec3(0);

                    for(currPos = startPos, currPos[workAxis2]++; 
                        currPos[workAxis2] < chunkSize && compareStep(startPos,currPos,direction,isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]];
                        currPos[workAxis2]++){}
                    quadSize[workAxis2] = currPos[workAxis2] - startPos[workAxis2];

                    for(currPos = startPos, currPos[workAxis1]++;
                        currPos[workAxis1] < chunkSize && compareStep(startPos,currPos,direction,isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]];
                        currPos[workAxis1]++){
                            for(currPos[workAxis2] = startPos[workAxis2]; currPos[workAxis2] < chunkSize && compareStep(startPos,currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]];
                            currPos[workAxis2]++){}

                            if(currPos[workAxis2]-startPos[workAxis2] < quadSize[workAxis2]){
                                break;
                            } else{
                                currPos[workAxis2] = startPos[workAxis2];
                            }
                        }
                    
                    quadSize[workAxis1] = currPos[workAxis1] - startPos[workAxis1];

                    m = glm::vec4(0.0f);
                    m[workAxis1] = (float) quadSize[workAxis1];

                    n = glm::vec4(0.0f);
                    n[workAxis2] = (float) quadSize[workAxis2];

                    offsetPos = glm::vec4(startPos.x,startPos.y,startPos.z,1.0f);
                    if(! isBackFace){
                        offsetPos[direction] += 1.0f;
                    }


        
                    mesh->addQuad(
                        model *offsetPos,
                        model *offsetPos+m,
                        model *offsetPos+m+n,
                        model *offsetPos+n,
                        quadColour,isBackFace);

                    for (int f = 0; f < quadSize[workAxis1]; f++) {
                        for (int g = 0; g < quadSize[workAxis2]; g++) {
                            merged[startPos[workAxis1] + f][ startPos[workAxis2] + g] = true;
                        }
                    }

                }
            }
        }
    }

}

void Chunk::renderChunkOutline() {
    std::vector<GLfloat> corners;
    // corners.push_back(chunkX*chunkSize);
    // corners.push_back(chunkY*chunkSize);
    // corners.push_back(chunkZ*chunkSize);

    // corners.push_back((chunkX+1)*chunkSize);
    // corners.push_back((chunkY)*chunkSize);
    // corners.push_back((chunkZ)*chunkSize);

    // corners.push_back((chunkX+1)*chunkSize);
    // corners.push_back((chunkY+1)*chunkSize);
    // corners.push_back((chunkZ)*chunkSize);

    // corners.push_back((chunkX)*chunkSize);
    // corners.push_back((chunkY+1)*chunkSize);
    // corners.push_back((chunkZ)*chunkSize);

    // corners.push_back((chunkX)*chunkSize);
    // corners.push_back((chunkY+1)*chunkSize);
    // corners.push_back((chunkZ+1)*chunkSize);

    // corners.push_back((chunkX)*chunkSize);
    // corners.push_back((chunkY)*chunkSize);
    // corners.push_back((chunkZ+1)*chunkSize);

    // corners.push_back((chunkX+1)*chunkSize);
    // corners.push_back((chunkY)*chunkSize);
    // corners.push_back((chunkZ+1)*chunkSize);

    // corners.push_back((chunkX+1)*chunkSize);
    // corners.push_back((chunkY+1)*chunkSize);
    // corners.push_back((chunkZ+1)*chunkSize);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, corners.size()*sizeof(GLfloat), &(corners[0]), GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, corners.size());
}



void Chunk::renderChunk(){

    // Bind the chunk's vertex array
    glBindVertexArrayOES( (mesh->vertex_array_buffer) );

    // Used for debugging
    // renderChunkOutline();
    
    if(!terrainPopulationFinished){ // can't yet build mesh
        return;
    }

    if( ! lastMeshStillValid){ // we have to generate the mesh
        
        isMeshInBuffer = false;

        // Start mesh generation on other thread
        doNotDelete = true; // don't delete this chunk whilst other threads are using it
        {
            std::lock_guard<std::mutex> lk(my_world->chunk_mesh_gen_m);
            my_world->chunks_for_mesh_gen.push(this);
        }
        // Wake up the thread to generate our mesh
        my_world->chunk_mesh_gen_cv.notify_one();

        lastMeshStillValid = true;
       
    }
    

    if(mesh->isFullyGenerated){
        if(mesh->vertices.size() == 0){ // Mesh is empty
            if( !declaredNoDraw){ // This is the first time we find the mesh empty
                interface->chunksNoDrawCall++;
                declaredNoDraw = true;
            }
            return;
        }

        if(!isMeshInBuffer){ // first time we have found the mesh to be generated
            mesh->bufferMesh();
            isMeshInBuffer = true;
        }

        // Draw the mesh
        glDrawElements(
            GL_TRIANGLES,
            (mesh->indices).size(),
            GL_UNSIGNED_INT,
            (void*)0
        );
    }
    
}

