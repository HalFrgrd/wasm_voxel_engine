#include "shader.h"

void Shader::initShader(std::string vertexPath, std::string fragmentPath){

    GLuint vertexShaderID = initSpecificShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = initSpecificShader(fragmentPath, GL_FRAGMENT_SHADER);

    
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
};

GLuint Shader::initSpecificShader(std::string path, GLenum shaderType){
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    //Source file loaded
    if( sourceFile )
    {
        //Get shader source
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
    //Create shader ID
        shaderID = glCreateShader( shaderType );

        //Set shader source
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

        //Compile shader source
        glCompileShader( shaderID );

        //Check shader for errors
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
        if( shaderCompiled != GL_TRUE )
        {
            printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
            // printShaderLog( shaderID );
            glDeleteShader( shaderID );
            shaderID = 0;
        }
    }
    else{
        printf( "Unable to open file %s\n", path.c_str() );
    }

    return shaderID;
}

void Shader::activate(){
    glUseProgram(shaderProgramID);
}

GLint Shader::getAttribLocation(const GLchar * attrib){
    return glGetAttribLocation(shaderProgramID, attrib);
};

GLint Shader::getUniformLocation(const GLchar * uniform){
    return glGetUniformLocation(shaderProgramID, uniform);
};

void Shader::setMat4(const GLchar *name, const glm::mat4 &mat){
    unsigned int location = glGetUniformLocation(shaderProgramID, name);

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

void Shader::setVec3(const GLchar *name, const glm::vec3 &vec){
    unsigned int location = glGetUniformLocation(shaderProgramID, name);
    glUniform3f(location, vec.x, vec.y, vec.z);
};
