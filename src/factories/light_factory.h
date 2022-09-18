#pragma once

#include "data_types/types.h"
#include <opengl-cpp/backend/gl.h>

namespace game_engine {

class light_factory_t {
  public:
    light_factory_t(opengl_cpp::gl_t &gl);
    light_pointer_t build_light(light_type_t type);

  private:
    opengl_cpp::gl_t &m_gl;
};

} // namespace game_engine
