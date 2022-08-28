#include "material.h"

namespace opengl_wrapper {

material::material(float specular, float shininess) : m_specular(specular), m_shininess(shininess) {
}

material &material::specular(float v) {
    m_specular = v;
    return *this;
}

float material::specular() const {
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
