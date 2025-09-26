#pragma once
#include <glad/glad.h>
#include <string>

namespace glutils{
    GLuint compileShader(GLenum, const std::string&);
    GLuint linkProgram(GLuint, GLuint);
    bool checkGLError(const std::string&);
}