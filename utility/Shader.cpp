#include "Shader.hpp"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

    // 1- Get shaders code
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        vertexCode = get_file_contents(vertexPath);
        fragmentCode = get_file_contents(fragmentPath);
    }
    catch (std::exception e)
    {
        std::cerr << "[SHADER] Vertex/fragment code not found" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar * fShaderCode = fragmentCode.c_str();

    // 2- Compile shaders
    GLint success;
    GLchar errorLog[512];

    // Vertex Shader
    vertexProgram = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexProgram, 1, &vShaderCode, NULL);
    glCompileShader(vertexProgram);

    glGetShaderiv(vertexProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexProgram, 512, NULL, errorLog);
        std::cerr << "[SHADER::VERTEX] Vertex code " << vertexPath << " compilation failed" << std::endl;
        std::cerr << errorLog << std::endl;
    }

    // Fragment Shader
    fragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentProgram, 1, &fShaderCode, NULL);
    glCompileShader(fragmentProgram);

    glGetShaderiv(fragmentProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentProgram, 512, NULL, errorLog);
        std::cerr << "[SHADER::FRAGMENT] Fragment code " << fragmentPath << " compilation failed" << std::endl;
        std::cerr << errorLog << std::endl;
    }

    // 3- Create program, attach shaders
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertexProgram);
    glAttachShader(this->Program, fragmentProgram);
}

// Link currently compiled program
void Shader::Link() {
    // 4- Link shaders
    glLinkProgram(this->Program);

    GLint success;
    GLchar errorLog[512];
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, errorLog);
        std::cerr << "[SHADER::PROGRAM] Program linking failed" << std::endl;
    }

    // Detach and delete the linked programs to save memory
    glDetachShader(this->Program, vertexProgram);
    glDetachShader(this->Program, fragmentProgram);
    glDeleteShader(vertexProgram);
    glDeleteShader(fragmentProgram);
}

// Uses the current shader
void Shader::Use() {
    glUseProgram(this->Program);
}

// Delete program
void Shader::Delete() {
    glDeleteProgram(Program);
}
