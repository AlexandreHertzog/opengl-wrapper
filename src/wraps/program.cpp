#include "program.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "shader.h"
#include "utils/utils.h"
#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace opengl_wrapper {

program::program() : shader_count_(0), id_(graphics::instance().gl_create_program()), linked_(false) {
    BOOST_LOG_TRIVIAL(trace) << "program::program " << *this;
}

program::program(program &&other) noexcept
    : shaders_(std::move(other.shaders_)), shader_count_(other.shader_count_), id_(other.id_), linked_(other.linked_) {

    BOOST_LOG_TRIVIAL(trace) << "program::program " << *this << " other=" << other;

    other.shader_count_ = 0;
    other.id_ = 0;
    other.linked_ = false;
}

program::~program() {
    BOOST_LOG_TRIVIAL(trace) << "program::~program " << *this;
    if (0 != id_) {
        graphics::instance().gl_delete_program(id_);
    }
}

program &program::operator=(program &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "program::operator= " << *this << " other=" << other;

    this->shaders_ = std::move(other.shaders_);

    this->shader_count_ = other.shader_count_;
    other.shader_count_ = 0;

    this->id_ = other.id_;
    other.id_ = 0;

    this->linked_ = other.linked_;
    other.linked_ = false;

    return *this;
}

bool program::operator==(const opengl_wrapper::program &other) const {
    BOOST_LOG_TRIVIAL(trace) << "program::operator== " << *this << " other=" << other;
    return this->id_ == other.id_;
}

void program::add_shader(shader shader) {
    BOOST_LOG_TRIVIAL(trace) << "program::add_shader " << *this << " shader= " << shader;
    if (0 == id_) {
        id_ = graphics::instance().gl_create_program();
        assert(0 == shader_count_);
    }

    graphics::instance().gl_attach_shader(id_, shader.get_id());
    shaders_.emplace_back(std::move(shader));
    shader_count_++;
}

void program::link() {
    BOOST_LOG_TRIVIAL(trace) << "program::link " << *this;
    constexpr auto error_string_length = 512;

    assert(0 != id_);

    graphics::instance().gl_link_program(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    graphics::instance().gl_get_programiv(id_, GL_LINK_STATUS, &success);
    if (GL_FALSE == success) {
        graphics::instance().gl_get_program_info_log(id_, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }

    shaders_.clear();
    linked_ = true;
}

void program::set_use_callback(opengl_wrapper::program::use_callback callback) {
    BOOST_LOG_TRIVIAL(trace) << "program::set_use_callback " << *this << " callback=" << &callback;
    use_callback_ = std::move(callback);
}

int program::get_uniform_location(const char *var_name) const {
    BOOST_LOG_TRIVIAL(trace) << "program::get_uniform_location " << *this << " var_name=" << std::quoted(var_name);
    return graphics::instance().gl_get_uniform_location(id_, var_name);
}

void program::set_uniform(const char *var_name, int value) {
    graphics::instance().gl_uniform1i(get_uniform_location(var_name), value);
}

void program::set_uniform(const char *var_name, const float *value) {
    graphics::instance().gl_uniform_matrix_4fv(get_uniform_location(var_name), 1, GL_FALSE, value);
}

void program::use() { // NOLINT(readability-make-member-function-const)
    BOOST_LOG_TRIVIAL(trace) << "program::use " << *this;
    graphics::instance().gl_use_program(id_);
    if (use_callback_) {
        use_callback_(*this);
    }
}

const std::vector<shader> &program::get_shaders() const {
    return shaders_;
}

GLuint program::get_id() const {
    return id_;
}

bool program::get_linked() const {
    return linked_;
}

const program::use_callback &program::get_use_callback() const {
    return use_callback_;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::program &p) {
    return os << "program" << parenthesis(&p) << " shaders=" << p.get_shaders() << ", id=" << p.get_id()
              << ", linked=" << p.get_linked() << ", callback=" << &p.get_use_callback();
}

} // namespace opengl_wrapper
