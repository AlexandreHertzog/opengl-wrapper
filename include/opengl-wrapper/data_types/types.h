#pragma once

#include <array>
#include <glm/glm.hpp>
#include <iomanip>
#include <memory>
#include <ostream>

namespace opengl_wrapper {

class texture_t;
using texture_pointer_t = std::shared_ptr<texture_t>;

using identifier_t = unsigned;

using color_alpha_t = std::array<float, 4>;
using direction_t = std::array<float, 3>;
using position_t = std::array<float, 3>;
using tex_coord_t = std::array<float, 2>;

enum class buffer_target_t {
    undefined = -1,
    simple_array = 0x8892,
    element_array = 0x8893
};

enum class shader_type_t {
    undefined = -1,
    fragment = 0x8B30,
    vertex = 0x8B31
};

enum class shader_parameter_t {
    undefined = -1,
    compile_status = 0x8B81
};

enum class texture_format_t {
    undefined = -1,
    rgb = 0x1907,
    rgba = 0x1908
};

enum class program_parameter_t {
    undefined = -1,
    link_status = 0x8B82
};

enum class texture_target_t {
    undefined = -1,
    tex_2d = 0x0DE1
};

enum class texture_parameter_t {
    undefined = -1,
    mag_filter = 0x2800,
    min_filter = 0x2801,
    wrap_s = 0x2802,
    wrap_t = 0x2803,
};

enum class texture_parameter_values_t {
    undefined = -1,
    linear = 0x2601,
    linear_mipmap_linear = 0x2703,
    repeat = 0x2901
};

enum class graphics_feature_t {
    undefined = -1,
    depth_test = 0x0B71,
};

enum class polygon_mode_t {
    undefined = -1,
    line = 0x1B01,
    fill = 0x1B02
};

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

struct vertex_t {
    position_t m_pos;
    tex_coord_t m_tex;
    direction_t m_nor;
};

inline std::ostream &operator<<(std::ostream &os, buffer_target_t t) {
    return os << std::hex << "0x" << static_cast<int>(t);
}

inline std::ostream &operator<<(std::ostream &os, texture_target_t t) {
    return os << std::hex << "0x" << static_cast<int>(t);
}

} // namespace opengl_wrapper
