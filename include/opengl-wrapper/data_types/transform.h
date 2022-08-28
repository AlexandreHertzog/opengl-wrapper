#pragma once

#include <glm/glm.hpp>

namespace opengl_wrapper {

class transform {
  public:
    transform &translate(glm::vec3 translation);
    transform &translate(float x, float y, float z);
    [[nodiscard]] const glm::vec3 &translation() const;
    transform &rotate(float angle, glm::vec3 axis);
    transform &rotate(float angle, float axis_x, float axis_y, float axis_z);
    [[nodiscard]] float rotation_angle() const;
    [[nodiscard]] const glm::vec3 &rotation_axis() const;
    transform &scale(glm::vec3 scale);
    transform &scale(float x, float y, float z);
    [[nodiscard]] const glm::vec3 &scale() const;

  private:
    glm::vec3 m_translation{0.0F};
    float m_rotation_angle{0.0F};
    glm::vec3 m_rotation_axis{1.0F};
    glm::vec3 m_scale{1.0F};
};

} // namespace opengl_wrapper
