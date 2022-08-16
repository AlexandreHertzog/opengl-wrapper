#include "shader.h"

#include "exceptions/gl_error.h"
#include "utils/utils.h"
#include <array>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <fstream>

namespace opengl_wrapper {

shader::shader(GLenum type, const char *source) : id_(glCreateShader(type)) {
    BOOST_LOG_TRIVIAL(trace) << "shader::shader " << *this << " type=" << type << " source=" << std::quoted(source);
    if (source != nullptr) {
        compile(source);
    }
}

shader::shader(GLenum type, const std::filesystem::path &shader_path) : id_(0) {
    BOOST_LOG_TRIVIAL(trace) << "shader::shader " << *this << " type=" << type
                             << " source=" << std::quoted(shader_path.string());

    std::ifstream shader_file(shader_path);
    if (!shader_file.is_open()) {
        throw exception("shader file not found: " + shader_path.string());
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    std::string code = shader_stream.str();

    id_ = glCreateShader(type);
    compile(code.c_str());
}

shader::shader(shader &&other) noexcept : id_(other.id_) {
    BOOST_LOG_TRIVIAL(trace) << "shader::shader " << *this << " other=" << other;
    other.id_ = 0;
}

shader::~shader() {
    BOOST_LOG_TRIVIAL(trace) << "shader::~shader " << *this;
    glDeleteShader(id_);
}

shader &shader::operator=(shader &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "shader::operator= " << *this << " other=" << other;
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

GLuint shader::get_id() const {
    return id_;
}

void shader::compile(const char *source) { // NOLINT(readability-make-member-function-const)
    BOOST_LOG_TRIVIAL(trace) << "shader::compile " << *this << " source=" << std::quoted(source);
    assert(nullptr != source);

    constexpr auto error_string_length = 512;

    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (GL_FALSE == success) {
        glGetShaderInfoLog(id_, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::shader &s) {
    return os << "shader" << parenthesis(&s) << " id=" << s.get_id();
}

} // namespace opengl_wrapper
