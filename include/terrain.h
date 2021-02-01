#ifndef TERRAIN_H
#define TERRAIN_H

#include "block.h"
#include "perlin_noise.h"

class TerrainGenerator {


public:
    FastNoiseLite noise;
    
    TerrainGenerator(){
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    }

    Block::BlockType getBlock(glm::ivec3 pos) {

        float temp_noise = noise.GetNoise((float)pos.x,(float)pos.y,(float)pos.z);

        if(temp_noise < 0.1){
            return Block::BLOCK_AIR;
        }
        return Block::BLOCK_STONE;
    }


};

#endif