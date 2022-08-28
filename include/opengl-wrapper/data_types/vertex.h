#pragma once

#include <array>
#include <glm/glm.hpp>

namespace opengl_wrapper {

struct vertex {
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_nor;
};

} // namespace opengl_wrapper
