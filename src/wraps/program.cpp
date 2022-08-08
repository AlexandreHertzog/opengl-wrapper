#include "program.h"

#include <algorithm>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {
Program::Program() { id_ = glCreateProgram(); }

Program::Program(Program &&other) { std::swap(this->id_, other.id_); }

Program::~Program() { glDeleteProgram(id_); }

void Program::addShader(Shader shader) {
    glAttachShader(id_, shader.getId());
    shaders_.emplace_back(std::move(shader));
}

void Program::link() {
    glLinkProgram(id_);

    int success = 0;
    char infoLog[512] = {'\0'};

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id_, 512, NULL, infoLog);
        throw GlError(infoLog);
    }

    shaders_.clear();
}

void Program::use() { glUseProgram(id_); }
} // namespace opengl_wrapper