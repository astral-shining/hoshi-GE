#pragma once
#include <string_view>
#include <SmartVector.hpp>

#include "Utility.hpp"

class Shader {
    inline static SmartVector<Shader*, true> list {};
    uint32_t compiled_shader_index;
    uint32_t vs;
    uint32_t fs;
    uint32_t program {};
    bool compiled {};
    const char* vSource;
    const char* fSource;
public:
    Shader(const char* vSource_, const char* fSource_);

    static void compileAll(void);
    void compile(void);
    void use(void);
    uint32_t getProgram(void) const;
    int getAttrib(const char* name) const;
    void destroy(void);
    ~Shader();
};
