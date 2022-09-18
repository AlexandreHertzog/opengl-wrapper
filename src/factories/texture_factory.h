#pragma once

#include "data_types/types.h"
#include <memory>
#include <opengl-cpp/texture.h>

namespace game_engine {

class texture_factory_t {
  public:
    texture_factory_t(opengl_cpp::gl_t &gl);

    texture_pointer_t get_base_texture();
    texture_pointer_t build_white_texture();
    texture_pointer_t build_blue_texture();
    texture_pointer_t build_orange_texture();
    texture_pointer_t build_red_texture();
    texture_pointer_t build_green_texture();
    texture_pointer_t build_diffuse_texture();
    texture_pointer_t build_specular_texture();

  private:
    opengl_cpp::gl_t &m_gl;
    texture_pointer_t m_base_texture;

    texture_pointer_t build_texture(const char *path, int texture_layer);
};

} // namespace game_engine
