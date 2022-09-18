#include "camera.h"

#include "utils/configuration.h"
#include <glm/ext/matrix_transform.hpp>

namespace game_engine {

camera_t::camera_t(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_position(position), m_front(front), m_up(up) {
}

glm::mat4 camera_t::look_at(glm::vec3 target) {
    return glm::lookAt(m_position, target, m_up);
}

void camera_t::step_front() {
    m_position = m_position + configuration::camera_speed * m_front;
}

void camera_t::step_back() {
    m_position = m_position - configuration::camera_speed * m_front;
}

void camera_t::step_left() {
    m_position = m_position - configuration::camera_speed * glm::normalize(glm::cross(m_front, m_up));
}

void camera_t::step_right() {
    m_position = m_position + configuration::camera_speed * glm::normalize(glm::cross(m_front, m_up));
}

const glm::vec3 &camera_t::get_position() const {
    return m_position;
}

const glm::vec3 &camera_t::get_front() const {
    return m_front;
}

void camera_t::set_front(double pitch, double yaw) {
    glm::vec3 direction;
    direction[0] = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction[1] = static_cast<float>(sin(glm::radians(pitch)));
    direction[2] = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    m_front = glm::normalize(direction);
}

} // namespace game_engine
