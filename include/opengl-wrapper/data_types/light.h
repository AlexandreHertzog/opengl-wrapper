#pragma once

#include "glm/glm.hpp"
#include "shape.h"

namespace opengl_wrapper {

struct light {
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    shape m_shape;
};

} // namespace opengl_wrapper
