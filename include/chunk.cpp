#include "chunk.h"


int Chunk::flattenCoords(int i, int j, int k){
    return i + chunkSize*j + chunkSize*chunkSize*k;
}

int Chunk::flattenCoords(glm::vec3 coords){
    return flattenCoords(coords.x,coords.y,coords.z);
};


glm::vec3 Chunk::unflattenCoords(int i){
    return glm::vec3(i% (chunkSize), (i/chunkSize)%chunkSize, (i/(chunkSize*chunkSize))%chunkSize  );
}

Block::BlockType Chunk::getBlockFromChunk(glm::vec3 coords){

    assert(coords.x >=0 && coords.x < chunkSize &&
        coords.y >=0 && coords.y < chunkSize &&
        coords.z >=0 && coords.z < chunkSize);
    int blockCoord = flattenCoords(coords);
    assert(blockCoord < chunkSize*chunkSize*chunkSize);
    return cubePositions[blockCoord]; 
}



Block::BlockType Chunk::getBlockFromWorld(glm::vec3 coords){
    // std::cout << coords.x << " " << coords.x << " " << coords.x << std::endl;
    // assert(1+1==3);
    if(coords.x >=0 && coords.x < chunkSize &&
        coords.y >=0 && coords.y < chunkSize &&
        coords.z >=0 && coords.z < chunkSize )
        {
            return getBlockFromChunk(coords);
        }

    assert(my_world.test == 3);
    // assert(0==1);
    
    return my_world.worldgetBlockFromWorld(chunkX*chunkSize + coords.x,chunkY*chunkSize + coords.y,chunkZ*chunkSize + coords.z );
};


void Chunk::setChunkCoords(int x, int y, int z){
    chunkX = x;
    chunkY = y;
    chunkZ = z;

}

Chunk::Chunk(World &initWorld) : my_world (initWorld) {

    

    for(int k = 0; k < chunkSize; k++){
        for(int i = 0; i < chunkSize; i++){
            for(int j = 0; j < chunkSize; j++){
                int heightThreshold = 4+ (rand()%4);
                if(j > heightThreshold){
                    cubePositions[flattenCoords(i,j,k)] = Block::BLOCK_AIR;
                } else{
                    if(rand()%5 == 1){
                        cubePositions[flattenCoords(i,j,k)] = Block::BLOCK_DIRT;
                    }else{
                        cubePositions[flattenCoords(i,j,k)] = Block::BLOCK_STONE;
                    }
                }
            }
        }
    }


}

bool Chunk::isBlockFaceVisible(glm::vec3 blockPos, int axis, bool isBackFace){
    glm::vec3 temp = blockPos;
    temp[axis] += isBackFace ? -1 : 1;
    return !Block::isSolid(getBlockFromWorld(temp));
}

bool Chunk::compareStep(glm::vec3 a, glm::vec3 b, int direction, bool isBackFace){
    Block::BlockType blockA = getBlockFromWorld(a);
    Block::BlockType blockB = getBlockFromWorld(b);

    return blockA == blockB && Block::isSolid(blockA) && isBlockFaceVisible(b, direction, isBackFace);
}


void Chunk::renderChunk(Renderer &renderer, Camera &camera){


    // https://eddieabbondanz.io/post/voxel/greedy-mesh/

    // glBindVertexArray(renderer.vao);
    glm::vec3 chunkShift = glm::vec3(chunkX * chunkSize, chunkY * chunkSize,chunkZ * chunkSize );
    mesh.vertices.clear();
    mesh.colours.clear();

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
                    

                    startBlock = getBlockFromChunk(startPos);

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
                    offsetPos[direction] += isBackFace ? 0.0f : 1.0f;

                    glm::vec4 quads[4];

                    glm::mat4 model = glm::mat4(1.0f); 
                    // glm::mat3 model = glm::mat3(1.0f); 
                    model = glm::translate(model, chunkShift);

                    quads[0] = model * offsetPos ;
                    quads[1] = model *offsetPos+m;
                    quads[2] = model *offsetPos+m+n;
                    quads[3] = model *offsetPos+n;

                    // glm::vec3 quadColour = Block::blockColour(startBlock);
        
                    mesh.addQuad(quads,quadColour,isBackFace);

                    for (int f = 0; f < quadSize[workAxis1]; f++) {
                        for (int g = 0; g < quadSize[workAxis2]; g++) {
                            merged[startPos[workAxis1] + f][ startPos[workAxis2] + g] = true;
                        }
                    }

                }
            }
        }
    }

    // for (unsigned int i = 0; i < chunkSize*chunkSize*chunkSize; i++)
    // {   
    //     if(cubePositions[i] != Block::BLOCK_AIR){

    //         glm::mat4 model = glm::mat4(1.0f); 
    //         model = glm::translate(model, unflattenCoords(i));
    //         model = glm::translate(model, chunkShift);

    //         glm::vec3 quads[4];

    //         quads[0] = model * glm::vec4(0.5,0.5,0.5,1.0);
    //         quads[1] = model * glm::vec4(-0.5,0.5,0.5,1.0);
    //         quads[2] = model * glm::vec4(0.5,-0.5,0.5,1.0);
    //         quads[3] = model * glm::vec4(-0.5,-0.5,0.5,1.0);

    //         mesh.addQuad(quads, false);


    //         // renderer.my_shader.setVec3("main_colour", glm::vec3(0.0,0.0,0.0));
    //         // glDrawArrays(GL_LINE_STRIP, 0, 36);
    //     }
    // }


    // glFrontFace(GL_CCW);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);

    


    glBindBuffer(GL_ARRAY_BUFFER, renderer.colours);
    glBufferData(GL_ARRAY_BUFFER, mesh.colours.size()*sizeof(float), &mesh.colours[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);
    
    // renderer.my_shader.setVec3("main_colour", glm::vec3(0.9,0.5,0.1));

    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    
    // renderer.my_shader.setVec3("main_colour", glm::vec3(0.0,0.0,0.0));
    // glDrawArrays(GL_LINES, 0, mesh.vertices.size());

}

void ChunkMesh::addVertex(glm::vec4 vertex, glm::vec3 colour){
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);

    colours.push_back(colour.x);
    colours.push_back(colour.y);
    colours.push_back(colour.z);
}

void ChunkMesh::addQuad(glm::vec4 quadVertices[],glm::vec3 colour, bool isBackFace ){
    // if (sizeof(quadVertices)/sizeof(quadVertices[0]) != 4){
    //     std::cout << "Length of vertices" << sizeof(quadVertices)/sizeof(quadVertices[0]) << std::endl; 
    //     throw "Given quad with not 4 vertices";
    // }

    if (!isBackFace) {
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[1], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[3], colour);
    } else {
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[1], colour);
        addVertex(quadVertices[0], colour);
        addVertex(quadVertices[3], colour);
        addVertex(quadVertices[2], colour);
        addVertex(quadVertices[0], colour);
    }
}
