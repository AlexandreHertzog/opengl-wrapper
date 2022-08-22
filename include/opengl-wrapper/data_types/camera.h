#pragma once

#include "glm/glm.hpp"

namespace opengl_wrapper {

class camera {
  public:
    explicit camera(glm::vec3 position = {}, glm::vec3 target = {});
    glm::mat4 look_at(glm::vec3 target);

    void set_position(glm::vec3 position);
    [[nodiscard]] const glm::vec3 &get_position() const;

    [[nodiscard]] const glm::vec3 &get_target() const;

    void set_up(glm::vec3 up);
    [[nodiscard]] const glm::vec3 &get_up() const;

  private:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_direction;
    glm::vec3 m_right;
    glm::vec3 m_up;
};

} // namespace opengl_wrapper
