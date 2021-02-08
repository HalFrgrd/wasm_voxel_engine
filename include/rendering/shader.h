#ifndef SHADER_H
#define SHADER_H

  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL.h>

#include <SDL_opengles2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
    
  
    // constructor reads and builds the shader
    void initShader(std::string vertexPath, std::string fragmentPath);

    // use/activate the shader
    void activate();

    // utility functions
    GLint getAttribLocation(const GLchar *attrib);
    GLint getUniformLocation(const GLchar *uniform);

    void setMat4(const GLchar *name, const glm::mat4 &mat);
    void setVec3(const GLchar *name, const glm::vec3 &vec);

    
    unsigned int shaderProgramID;

private:
    GLuint initSpecificShader(std::string path, GLenum shaderType);

    // the program ID
};
  
#endif