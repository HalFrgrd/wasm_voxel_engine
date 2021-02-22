#ifndef TERRAIN_H
#define TERRAIN_H

#include "block.h"
#include "fastnoiselite.h"

class TerrainGenerator {


public:
    FastNoiseLite noise;
    
    TerrainGenerator(){
        noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    }

    Block::BlockType getBlock(glm::ivec3 pos) {

        float temp_noise = noise.GetNoise((float)pos.x,(float)pos.z);

        if(pos.y  + temp_noise*30 > 0){ 
            return Block::BLOCK_AIR;
        }
        return Block::BLOCK_STONE;
    }


};

#endif