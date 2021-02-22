#include "chunk.h"



int Chunk::flattenCoords(int i, int j, int k){
    int val = i + chunkSize*j + chunkSize*chunkSize*k;
    // if(val < 0 || val >= chunkSize*chunkSize*chunkSize){
    //     std::cout<<val<<i<<j<<k<<std::endl;
    // }
    return val;
}

int Chunk::flattenCoords(glm::ivec3 coords){
    return flattenCoords(coords.x,coords.y,coords.z);
};


glm::ivec3 Chunk::unflattenCoords(int i){
    return glm::ivec3(i% (chunkSize), (i/chunkSize)%chunkSize, (i/(chunkSize*chunkSize))%chunkSize  );
}

glm::ivec3 Chunk::localPosToGlobalPos(int x, int y, int z){
    return glm::ivec3(chunkX*chunkSize + x,chunkY*chunkSize + y,chunkZ*chunkSize + z);
}

Block::BlockType Chunk::getBlockFromChunk(glm::ivec3 coords){
    Block::BlockType return_block = cubePositions[flattenCoords(coords)];
    // if(return_block != 0 || return_block != 3){
    //     std::cout<< return_block <<std::endl;
    //     std::cout<<coords.x<<" "<<coords.y<<" "<<coords.z<<std::endl;
    // }
    return return_block; 
}



Block::BlockType Chunk::getBlockFromWorld(glm::ivec3 coords){

    if(coords.x >=0 && coords.x < chunkSize &&
        coords.y >=0 && coords.y < chunkSize &&
        coords.z >=0 && coords.z < chunkSize )
        {
            return getBlockFromChunk(coords);
        }

    // return Block::BLOCK_AIR;
    // return terr
    return my_world->worldgetBlockFromWorld(chunkX*chunkSize + coords.x,chunkY*chunkSize + coords.y,chunkZ*chunkSize + coords.z );
};


void Chunk::setChunkCoords(int x, int y, int z){
    chunkX = x;
    chunkY = y;
    chunkZ = z;
}

// Chunk::Chunk(World& initWorld): my_world(initWorld) {
Chunk::Chunk(World *initWorld, GUI_Interface *initInterface, int _chunkX, int _chunkY, int _chunkZ, TerrainGenerator* initTerrain, Renderer *initRenderer) {
    
    mesh = new ChunkMesh(initRenderer);

    my_world = initWorld;
    interface = initInterface;
    terrain = initTerrain;

    chunkX = _chunkX;
    chunkY = _chunkY;
    chunkZ = _chunkZ;

    // Start mesh generation on other thread
    {
        std::lock_guard<std::mutex> lk(my_world->chunks_for_init_m);
        my_world->chunks_for_init.push(this);
    }
    my_world->chunks_for_init_cv.notify_one();


}

void Chunk::initialiseTerrain() {
    int block_coords = 0;
    for(int k = 0; k < chunkSize; k++){
        for(int i = 0; i < chunkSize; i++){
            for(int j = 0; j < chunkSize; j++){
                block_coords = flattenCoords(i,j,k);
                cubePositions[block_coords] = terrain->getBlock(localPosToGlobalPos(i,j,k));
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

    glm::vec3 chunkShift = glm::vec3(chunkX * chunkSize, chunkY * chunkSize, chunkZ *chunkSize );
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



void Chunk::renderChunk( Camera &camera){

    if(!terrainPopulationFinished){ // can't yet build mesh
        return;
    }

    // Bind the chunk's vertex array
    glBindVertexArrayOES( (mesh->vertex_array_buffer) );

    if( ! lastMeshStillValid){ // we have to generate the mesh
        
        meshUnbuffered = false;

        // Start mesh generation on other thread
        {
            std::lock_guard<std::mutex> lk(my_world->chunk_mesh_gen_m);
            my_world->chunks_for_mesh_gen.push(this);
        }
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

        if(!meshUnbuffered){ // first time we have found the mesh to be generated
            glBindBuffer(GL_ARRAY_BUFFER, mesh->colour_buffer);
            glBufferData(GL_ARRAY_BUFFER, (mesh->colours).size()*sizeof(GLfloat), &(mesh->colours[0]), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, (mesh->vertices).size()*sizeof(GLfloat), &(mesh->vertices[0]), GL_STATIC_DRAW);
            meshUnbuffered = true;
        }
        glDrawArrays(GL_TRIANGLES, 0, (mesh->vertices).size()*sizeof(GLfloat)/3);
    }
    
}

