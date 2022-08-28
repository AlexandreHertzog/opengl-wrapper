#include "transform.h"

namespace opengl_wrapper {

transform &transform::translate(glm::vec3 translation) {
    m_translation = std::move(translation);
    return *this;
}

transform &transform::translate(float x, float y, float z) {
    m_translation = glm::vec3(x, y, z);
    return *this;
}

const glm::vec3 &transform::translation() const {
    return m_translation;
}

transform &transform::rotate(float angle, glm::vec3 axis) {
    m_rotation_angle = angle;
    m_rotation_axis = std::move(axis);
    return *this;
}

transform &transform::rotate(float angle, float axis_x, float axis_y, float axis_z) {
    m_rotation_angle = angle;
    m_rotation_axis = glm::vec3(axis_x, axis_y, axis_z);
    return *this;
}

float transform::rotation_angle() const {
    return m_rotation_angle;
}

const glm::vec3 &transform::rotation_axis() const {
    return m_rotation_axis;
}

transform &transform::scale(glm::vec3 scale) {
    m_scale = std::move(scale);
    return *this;
}

transform &transform::scale(float x, float y, float z) {
    m_scale = glm::vec3(x, y, z);
    return *this;
}

const glm::vec3 &transform::scale() const {
    return m_scale;
}

} // namespace opengl_wrapper
