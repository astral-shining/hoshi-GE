#include "Shader.hpp"
#include <GL/glew.h>


void Shader::compileAll() {
    for (auto& p : Shader::list) {
        p->compile();
    }
}

Shader::Shader(const char* vSource_, const char* fSource_) : vSource(vSource_), fSource(fSource_) {
}

void Shader::compile() {
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
}

Shader::~Shader() {
    if (compiled) {
        destroy();
    }
}
