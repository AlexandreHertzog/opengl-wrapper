#pragma once

#include <glm/glm.hpp>

namespace opengl_wrapper {

struct transform {
    glm::vec3 m_translation{0.0F};
    float m_rotation_angle{0.0F};
    glm::vec3 m_rotation_axis{1.0F};
    glm::vec3 m_scale{1.0F};
};

} // namespace opengl_wrapper
