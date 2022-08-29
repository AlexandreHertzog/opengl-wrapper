#pragma once

#include <glm/glm.hpp>

namespace opengl_wrapper {

struct material {
    glm::vec3 m_ambient{0.3F};
    glm::vec3 m_diffuse{1.0F};
    glm::vec3 m_specular{0.5F};
    float m_shininess{32.0F};
};

} // namespace opengl_wrapper
