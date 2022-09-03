#pragma once

#include "glm/glm.hpp"
#include "shape.h"

namespace opengl_wrapper {

struct light {
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_attenuation_constant{};
    float m_attenuation_linear{};
    float m_attenuation_quadratic{};
    shape m_shape;

    virtual ~light() = default;
};

struct directional_light : light {
    glm::vec3 m_direction;
};

struct spot_light : light {
    glm::vec3 m_direction;
    float m_cutoff_begin{};
    float m_cutoff_end{};
};

} // namespace opengl_wrapper
