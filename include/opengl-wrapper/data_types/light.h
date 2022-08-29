#pragma once

#include "glm/glm.hpp"
#include "shape.h"

namespace opengl_wrapper {

struct light {
    glm::vec3 m_ambient{0.2F, 0.2F, 0.2F};
    glm::vec3 m_diffuse{1.0F, 1.0F, 1.0F};
    glm::vec3 m_specular{0.2F, 0.2F, 0.2F};
    shape m_shape;
};

} // namespace opengl_wrapper
