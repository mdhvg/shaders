#include "Shader.h"
#include "Debug.h"

#include <fstream>
#include <cassert>
#include <glad/glad.h>
#include <iostream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::cout << "Reading file: " << filename << std::endl;
    assert(file.is_open());
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

unsigned int compileShader(const std::string& filename, unsigned int type) {
    unsigned int id = glCreateShader(type);
    std::string source = readFile(filename);
    const char* sourcePtr = source.c_str();
    GLCall(glShaderSource(id, 1, &sourcePtr, nullptr));
    GLCall(glCompileShader(id));
    int compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    char infoLog[512];
    if (!compileStatus) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        assert(false);
    }
    return id;
}

unsigned int createRenderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int id = glCreateProgram();
    GLCall(glAttachShader(id, vertexShader));
    GLCall(glAttachShader(id, fragmentShader));
    GLCall(glLinkProgram(id));
    int linkStatus;
    glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
    char infoLog[512];
    if (!linkStatus) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(false);
    }
    return id;
}

unsigned int createComputeProgram(unsigned int computeShader) {
    unsigned int id = glCreateProgram();
    GLCall(glAttachShader(id, computeShader));
    GLCall(glLinkProgram(id));
    int linkStatus;
    glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
    char infoLog[512];
    if (!linkStatus) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(false);
    }
    return id;
}