#pragma once

#include "texture.h"
#include <glm/glm.hpp>

namespace opengl_wrapper {

struct material {
    glm::vec3 m_ambient{0.3F};
    glm::vec3 m_diffuse{1.0F};
    glm::vec3 m_specular{0.5F};
    float m_shininess{32.0F};
    texture::pointer_t m_texture1;
    texture::pointer_t m_texture2;
    float m_texture_mix{0.8F};
};

} // namespace opengl_wrapper
