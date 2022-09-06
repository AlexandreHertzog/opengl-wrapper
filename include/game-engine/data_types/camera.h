#pragma once

#include "glm/glm.hpp"

namespace game_engine {

class camera_t {
  public:
    explicit camera_t(glm::vec3 position = {}, glm::vec3 front = {}, glm::vec3 up = {});
    glm::mat4 look_at(glm::vec3 target);

    void set_position(glm::vec3 position);
    [[nodiscard]] const glm::vec3 &get_position() const;
    [[nodiscard]] const glm::vec3 &get_front() const;
    [[nodiscard]] const glm::vec3 &get_up() const;

    void set_front(double pitch, double yaw);

  private:
    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_front;
};

} // namespace game_engine
