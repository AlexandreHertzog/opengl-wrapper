#include "shader.h"

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Shader::Shader(GLenum type, const char *source) {
    id_ = glCreateShader(type);

    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    {
        int success = 0;
        char infoLog[512] = {'\0'};

        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            throw GlError(infoLog);
        }
    }
}

Shader::Shader(Shader &&other) noexcept { std::swap(this->id_, other.id_); }

Shader::~Shader() { glDeleteShader(id_); }

Shader &Shader::operator=(Shader &&other) noexcept {
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

GLuint Shader::getId() const { return id_; }

} // namespace opengl_wrapper