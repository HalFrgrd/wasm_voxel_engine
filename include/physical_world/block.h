#ifndef BLOCK_H
#define BLOCK_H

#include <SDL.h>
#include <SDL_opengles2.h>


namespace Block{
    enum BlockType: int {
        BLOCK_AIR = 0,
        BLOCK_DIRT ,
        BLOCK_GRASS ,
        BLOCK_STONE
    };

    inline glm::vec3 blockColour(BlockType block){
        switch (block)
        {
        case BLOCK_DIRT:
            return glm::vec3(0.8f,0.5f,0.0f);
            break;
        case BLOCK_STONE:
            return glm::vec3(0.3f,0.3f,0.3f);
            break;
        
        default:
            return glm::vec3(0.1f,0.1f,0.1f);
            break;
        }
    }

    inline bool isSolid(BlockType block){
        return block == BLOCK_STONE;
    }

    const GLfloat vertices[10*6*3] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f
    };
};

#endif