#include "opengl-wrapper/data_types/program.h"

#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <algorithm>
#include <cassert>

namespace opengl_wrapper {

program::program() : m_shader_count(0), m_id(graphics::instance().gl_create_program()), m_linked(false) {
}

program::program(program &&other) noexcept
    : m_shaders(std::move(other.m_shaders)), m_shader_count(other.m_shader_count), m_id(other.m_id),
      m_linked(other.m_linked) {

    other.m_shader_count = 0;
    other.m_id = 0;
    other.m_linked = false;
}

program::~program() {
    if (0 != m_id) {
        graphics::instance().gl_delete_program(m_id);
    }
}

program &program::operator=(program &&other) noexcept {
    this->m_shaders = std::move(other.m_shaders);

    this->m_shader_count = other.m_shader_count;
    other.m_shader_count = 0;

    this->m_id = other.m_id;
    other.m_id = 0;

    this->m_linked = other.m_linked;
    other.m_linked = false;

    return *this;
}

bool program::operator==(const opengl_wrapper::program &other) const {
    return this->m_id == other.m_id;
}

void program::add_shader(shader shader) {
    if (0 == m_id) {
        m_id = graphics::instance().gl_create_program();
        assert(0 == m_shader_count);
    }

    graphics::instance().gl_attach_shader(m_id, shader.get_id());
    m_shaders.emplace_back(std::move(shader));
    m_shader_count++;
}

void program::link() {
    constexpr auto error_string_length = 512;

    assert(0 != m_id);

    graphics::instance().gl_link_program(m_id);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    graphics::instance().gl_get_programiv(m_id, GL_LINK_STATUS, &success);
    if (GL_FALSE == success) {
        graphics::instance().gl_get_program_info_log(m_id, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }

    m_shaders.clear();
    m_linked = true;
}

void program::set_use_callback(opengl_wrapper::program::use_callback callback) {
    m_use_callback = std::move(callback);
}

int program::get_uniform_location(const char *var_name) const {
    return graphics::instance().gl_get_uniform_location(m_id, var_name);
}

void program::set_uniform(const char *var_name, int value) {
    graphics::instance().gl_uniform1i(get_uniform_location(var_name), value);
}

void program::set_uniform(const char *var_name, const float *value) {
    graphics::instance().gl_uniform_matrix_4fv(get_uniform_location(var_name), 1, GL_FALSE, value);
}

void program::use() { // NOLINT(readability-make-member-function-const)
    graphics::instance().gl_use_program(m_id);
    if (m_use_callback) {
        m_use_callback(*this);
    }
}

const std::vector<shader> &program::get_shaders() const {
    return m_shaders;
}

GLuint program::get_id() const {
    return m_id;
}

bool program::get_linked() const {
    return m_linked;
}

const program::use_callback &program::get_use_callback() const {
    return m_use_callback;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::program &p) {
    return os << "program" << parenthesis(&p) << " shaders=" << p.get_shaders() << ", id=" << p.get_id()
              << ", linked=" << p.get_linked() << ", callback=" << &p.get_use_callback();
}

} // namespace opengl_wrapper
