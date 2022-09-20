#pragma once

#include "data_types/types.h"
#include "factories/program_factory.h"

namespace game_engine {

class shape_manager_t {
  public:
    using pair_t = std::pair<program_pointer_t, shape_pointer_t>;
    using vector_t = std::vector<pair_t>;

    shape_manager_t(opengl_cpp::gl_t &gl);

    void add_object_shape(shape_pointer_t shape);
    void add_light_shape(shape_pointer_t shape);

    vector_t::iterator begin();
    vector_t::iterator end();

  private:
    opengl_cpp::gl_t &m_gl;
    program_factory_t m_program_factory;

    vector_t m_values;
    program_pointer_t m_object_program;
    program_pointer_t m_light_program;

    void add_shape(shape_pointer_t shape, program_pointer_t program);
};

} // namespace game_engine
