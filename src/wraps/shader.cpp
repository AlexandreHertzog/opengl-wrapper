#include "shader.h"

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Shader::Shader(GLenum type, const char *source) {
    id_ = glCreateShader(type);

    glShaderSource(id_, 1, &source, NULL);
    glCompileShader(id_);

    {
        int success = 0;
        char infoLog[512] = {'\0'};

        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, NULL, infoLog);
            throw GlError(infoLog);
        }
    }
}

Shader::Shader(Shader &&other) { std::swap(this->id_, other.id_); }

Shader::~Shader() { glDeleteShader(id_); }

Shader &Shader::operator=(Shader &&other) { this->id_ = std::move(other.id_); }

GLuint Shader::getId() const { return id_; }

} // namespace opengl_wrapper