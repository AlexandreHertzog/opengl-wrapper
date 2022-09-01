#include "opengl-wrapper/data_types/program.h"

#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <algorithm>
#include <cassert>

namespace opengl_wrapper {

program::program() : m_shader_count(0), m_id(graphics::instance().new_program()), m_linked(false) {
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
        graphics::instance().delete_program(m_id);
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
        m_id = graphics::instance().new_program();
        assert(0 == m_shader_count);
    }

    graphics::instance().attach_shader(*this, shader);
    m_shaders.emplace_back(std::move(shader));
    m_shader_count++;
}

void program::link() {
    assert(0 != m_id);

    graphics::instance().link(*this);

    const auto success = graphics::instance().get_parameter(*this, program_parameter_t::link_status);
    if (GL_FALSE == success) {
        throw gl_error(graphics::instance().get_info_log(*this));
    }

    m_shaders.clear();
    m_linked = true;
}

void program::set_use_callback(opengl_wrapper::program::use_callback callback) {
    m_use_callback = std::move(callback);
}

int program::get_uniform_location(const char *var_name) const {
    return graphics::instance().get_uniform_location(*this, var_name);
}

void program::set_uniform(const char *var_name, float value) {
    graphics::instance().set_uniform(get_uniform_location(var_name), value);
}

void program::set_uniform(const char *var_name, const glm::vec3 &vec) {
    graphics::instance().set_uniform(get_uniform_location(var_name), std::array<float, 3>{vec.x, vec.y, vec.z});
}

void program::set_uniform(const char *var_name, const std::array<float, 3> &v) {
    graphics::instance().set_uniform(get_uniform_location(var_name), v);
}

void program::set_uniform(const char *var_name, const std::array<float, 4> v) {
    graphics::instance().set_uniform(get_uniform_location(var_name), v);
}

void program::set_uniform(const char *var_name, int value) {
    graphics::instance().set_uniform(get_uniform_location(var_name), value);
}

void program::set_uniform(const char *var_name, const float *value) {
    graphics::instance().set_matrix4_uniform(get_uniform_location(var_name), 1, value);
}

void program::use(shape &s) { // NOLINT(readability-make-member-function-const)
    graphics::instance().use(*this);
    if (m_use_callback) {
        m_use_callback(*this, s);
    }
}

const std::vector<shader> &program::get_shaders() const {
    return m_shaders;
}

identifier_t program::get_id() const {
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
