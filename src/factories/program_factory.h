#pragma once

#include "data_types/types.h"
#include <memory>
#include <opengl-cpp/program.h>

namespace game_engine {

class program_factory_t {
  public:
    program_factory_t(opengl_cpp::gl_t &gl);
    program_pointer_t build_object_program();
    program_pointer_t build_light_program();

  private:
    opengl_cpp::gl_t &m_gl;

    program_pointer_t build_program(const char *vert_source_path, const char *frag_source_path);
};

} // namespace game_engine
