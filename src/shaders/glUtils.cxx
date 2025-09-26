#include "glUtils.hxx"
#include <iostream>

GLuint glUtils::compileShader(GLenum shaderType, const std::string& source){
    GLuint shader = glCreateShader(shaderType);
    const char *data = src.c_str();
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status){
        GLint len=0;
        glGetShaderiv(shader, GL_INFO_LENGTH, &len);
        std::string log(len,'\0');
        glGetShaderInfoLog(shader, len, nullptr, &log[0]);
        std::cerr<<"Shader compile error: "<<log<std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;

}

GLuint glUtils::linkProgram(GLuint vertexShader, GLuint fragmentShader){
    GLuint prog = glCreateProgram(); //creates program object and returns ID to attach shader objects
    glAttachShader(prog, vertexShader); // attaching vertex shader objects to the program
    glAttachShader(prog, fragmentShader); // attaching fragment shader objects to the program
    glLinkProgram(prog); // links to program object "prog" along with it's shaders by creating executables for each type
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if(!status){
        GLint len=0;
        glGetProgramiv(prog,GL_INFO_LOG_LENGTH,&len);
        std::string log(len,'\0');
        glGetProgramInfoLog(prog,len,nullptr,&log[0]);
        std::cerr<<"Program link error: "<<log<<std::endl;
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

bool glUtils::checkGLError(const std::string& context);{
    GLenum error = glGetError();
    if(e!=GL_NO_ERROR){
        std::cerr << "GL Error ("<<context<<"): 0x" << std::hex << e << std::dec << std::endl;;
        return true;
    }
    return false;
}