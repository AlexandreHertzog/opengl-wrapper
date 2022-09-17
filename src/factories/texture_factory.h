#pragma once

#include "data_types/types.h"
#include <memory>
#include <opengl-cpp/texture.h>

namespace game_engine {

class texture_factory_t {
  public:
    texture_factory_t(opengl_cpp::gl_t &gl);
    texture_pointer_t build_texture(const char *path, int texture_layer);

  private:
    opengl_cpp::gl_t &m_gl;
};

} // namespace game_engine
