#include "camera.h"
#include "glm/ext/matrix_transform.hpp"

namespace {

constexpr auto absolute_up = glm::vec3(0.0, 1.0, 0.0);

} // namespace

namespace opengl_wrapper {

camera::camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_position(position), m_front(front), m_up(up) {
}

glm::mat4 camera::look_at(glm::vec3 target) {
    return glm::lookAt(m_position, target, m_up);
}

void camera::set_position(glm::vec3 position) {
    m_position = position;
}

const glm::vec3 &camera::get_position() const {
    return m_position;
}

const glm::vec3 &camera::get_front() const {
    return m_front;
}

void camera::set_up(glm::vec3 up) {
    m_up = up;
}

const glm::vec3 &camera::get_up() const {
    return m_up;
}

void camera::set_front(double pitch, double yaw) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front = glm::normalize(direction);
}

} // namespace opengl_wrapper
