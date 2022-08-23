#include "camera.h"
#include "glm/ext/matrix_transform.hpp"

namespace {

constexpr auto absolute_up = glm::vec3(0.0, 1.0, 0.0);

} // namespace

namespace opengl_wrapper {

camera::camera(glm::vec3 position, glm::vec3 target, glm::vec3 up) : m_position(position), m_target(target), m_up(up) {
}

glm::mat4 camera::look_at(glm::vec3 target) {
    m_target = target;
    return glm::lookAt(m_position, m_target, m_up);
}

void camera::set_position(glm::vec3 position) {
    m_position = position;
    const auto direction = glm::normalize(m_position - m_target);
    const auto right = glm::normalize(glm::cross(absolute_up, direction));
    m_up = glm::cross(direction, right);
}

const glm::vec3 &camera::get_position() const {
    return m_position;
}

const glm::vec3 &camera::get_target() const {
    return m_target;
}

void camera::set_up(glm::vec3 up) {
    m_up = up;
}

const glm::vec3 &camera::get_up() const {
    return m_up;
}

} // namespace opengl_wrapper
