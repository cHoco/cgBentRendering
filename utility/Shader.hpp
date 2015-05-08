#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <iostream>

#include <GL/glew.h>

#include "fileUtils.hpp"

class Shader
{
public:
    // Shader program identifier
    GLuint Program;

    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    // Link currently loaded shader
    void Link();

    // Uses the current shader
    void Use();

    // Delete program
    void Delete();
private:
    GLuint vertexProgram;
    GLuint fragmentProgram;
};

#endif
