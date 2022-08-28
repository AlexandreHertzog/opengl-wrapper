#include "material.h"

namespace opengl_wrapper {

material &material::ambient(glm::vec3 v) {
    m_ambient = std::move(v);
    return *this;
}

const glm::vec3 &material::ambient() const {
    return m_ambient;
}

material &material::diffuse(glm::vec3 v) {
    m_diffuse = std::move(v);
    return *this;
}

const glm::vec3 &material::diffuse() const {
    return m_diffuse;
}

material &material::specular(glm::vec3 v) {
    m_specular = std::move(v);
    return *this;
}

const glm::vec3 &material::specular() const {
    return m_specular;
}

material &material::shininess(float v) {
    m_shininess = v;
    return *this;
}

float material::shininess() const {
    return m_shininess;
}

} // namespace opengl_wrapper
