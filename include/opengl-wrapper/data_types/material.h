#pragma once

#include <glm/glm.hpp>

namespace opengl_wrapper {

class material {
  public:
    material &ambient(glm::vec3 v);
    [[nodiscard]] const glm::vec3 &ambient() const;

    material &diffuse(glm::vec3 v);
    [[nodiscard]] const glm::vec3 &diffuse() const;

    material &specular(glm::vec3 v);
    [[nodiscard]] const glm::vec3 &specular() const;

    material &shininess(float v);
    [[nodiscard]] float shininess() const;

  private:
    glm::vec3 m_ambient{0.3F};
    glm::vec3 m_diffuse{1.0F};
    glm::vec3 m_specular{0.5F};
    float m_shininess{32.0F};
};

} // namespace opengl_wrapper
