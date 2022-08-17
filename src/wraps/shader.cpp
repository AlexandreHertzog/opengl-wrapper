#include "shader.h"

#include "api.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <array>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <fstream>

namespace opengl_wrapper {

shader::shader(GLenum type, const char *source) : id_(api::instance().gl_create_shader(type)) {
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

    id_ = api::instance().gl_create_shader(type);
    compile(code.c_str());
}

shader::shader(shader &&other) noexcept : id_(other.id_) {
    BOOST_LOG_TRIVIAL(trace) << "shader::shader " << *this << " other=" << other;
    other.id_ = 0;
}

shader::~shader() {
    BOOST_LOG_TRIVIAL(trace) << "shader::~shader " << *this;
    api::instance().gl_delete_shader(id_);
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

    api::instance().gl_shader_source(id_, 1, &source, nullptr);
    api::instance().gl_compile_shader(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    api::instance().gl_get_shaderiv(id_, GL_COMPILE_STATUS, &success);
    if (GL_FALSE == success) {
        api::instance().gl_get_shader_info_log(id_, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::shader &s) {
    return os << "shader" << parenthesis(&s) << " id=" << s.get_id();
}

} // namespace opengl_wrapper
