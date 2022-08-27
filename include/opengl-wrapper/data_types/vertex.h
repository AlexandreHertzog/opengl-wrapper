#pragma once

#include <array>
#include <glm/glm.hpp>

namespace opengl_wrapper {

struct vertex {
    glm::vec3 m_pos;
    glm::vec3 m_col;
    glm::vec2 m_tex;
};

} // namespace opengl_wrapper
