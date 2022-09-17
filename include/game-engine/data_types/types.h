#pragma once

#include <array>
#include <glm/glm.hpp>
#include <iomanip>
#include <memory>
#include <opengl-cpp/texture.h>

namespace game_engine {

struct light_t;

using texture_pointer_t = std::shared_ptr<opengl_cpp::texture_t>;
using program_pointer_t = std::shared_ptr<opengl_cpp::program_t>;
using texture_pointer_t = std::shared_ptr<opengl_cpp::texture_t>;
using light_pointer_t = std::shared_ptr<light_t>;

struct light_t {
    glm::vec3 m_position{};
    glm::vec3 m_ambient{};
    glm::vec3 m_diffuse{};
    glm::vec3 m_specular{};
    float m_attenuation_constant{};
    float m_attenuation_linear{};
    float m_attenuation_quadratic{};

    light_t() = default;
    virtual ~light_t() = default;
    light_t(const light_t &) = default;
    light_t(light_t &&) = default;
    light_t &operator=(const light_t &) = default;
    light_t &operator=(light_t &&) = default;
};

struct directional_light_t : light_t {
    glm::vec3 m_direction;
};

struct spot_light_t : light_t {
    glm::vec3 m_direction;
    float m_cutoff_begin{};
    float m_cutoff_end{};
};

enum class light_type_t {
    deactivated = 0,
    ambient = 1,
    directional = 2,
    spot = 3
};

struct material_t {
    glm::vec3 m_ambient{};
    texture_pointer_t m_diffuse{};
    texture_pointer_t m_specular{};
    float m_shininess{};
    texture_pointer_t m_texture1{};
    texture_pointer_t m_texture2{};
    float m_texture_mix{};
};

struct transform_t {
    glm::vec3 m_translation{0.0F};
    float m_rotation_angle{0.0F};
    glm::vec3 m_rotation_axis{1.0F};
    glm::vec3 m_scale{1.0F};
};

} // namespace game_engine
