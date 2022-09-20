#include "shape_manager.h"

namespace game_engine {

shape_manager_t::shape_manager_t(opengl_cpp::gl_t &gl)
    : m_gl(gl), m_program_factory(m_gl), m_object_program(m_program_factory.build_object_program()),
      m_light_program(m_program_factory.build_light_program()) {
}

void shape_manager_t::add_object_shape(shape_pointer_t shape) {
    add_shape(std::move(shape), m_object_program);
}

void shape_manager_t::add_light_shape(shape_pointer_t shape) {
    add_shape(std::move(shape), m_light_program);
}

std::vector<shape_manager_t::pair_t>::iterator shape_manager_t::begin() {
    return m_values.begin();
}

std::vector<shape_manager_t::pair_t>::iterator shape_manager_t::end() {
    return m_values.end();
}

void shape_manager_t::add_shape(shape_pointer_t shape, program_pointer_t program) {
    auto find = std::find_if(m_values.begin(), m_values.end(), [&](pair_t &v) {
        return v.second == shape;
    });
    if (m_values.end() == find) {
        m_values.emplace_back(std::make_pair(std::move(program), std::move(shape)));
    }
}
} // namespace game_engine
