#include "Shader.hpp"
#include <GL/glew.h>

Shader::Shader(const char* vSource_, const char* fSource_) : vSource(vSource_), fSource(fSource_) {}

Shader& Shader::compile() {
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glShaderSource(vs, 1, &vSource, NULL);
    glShaderSource(fs, 1, &fSource, NULL);
    glCompileShader(vs);
    glCompileShader(fs);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        terminate("ERROR: Compiling vertex shader ", infoLog);
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        terminate("ERROR: Compiling fragment shader ", infoLog);
    }

    // Link shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    compiled = true;
    compiled_shader_index = compiled_shaders.size();
    compiled_shaders.push_back(this);
    return *this;
}

void Shader::use() {
    glUseProgram(program);
}

uint32_t Shader::getProgram() const {
    return program;
}

int Shader::getAttrib(const char* name) const {
    return glGetAttribLocation(program, name);
}

void Shader::destroy() {
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);

    compiled_shaders[compiled_shader_index] = compiled_shaders[compiled_shaders.size()-1];
    compiled_shaders.pop_back();
}

Shader::~Shader() {
    if (compiled) {
        destroy();
    }
}