#pragma once

#include "data_types/types.h"

namespace game_engine::configuration {

constexpr glm::vec3 camera_start_position = {0.0, 3.0, 8.0};
constexpr glm::vec3 camera_start_front = {0.0, -0.3, -1.0};
constexpr glm::vec3 camera_start_up = {0.0, 1.0, 0.0};
constexpr auto camera_max_angle = 89.0;
constexpr auto camera_min_angle = -89.0;
constexpr auto camera_clipping_near = 0.1F;
constexpr auto camera_clipping_far = 100.0F;
constexpr auto camera_clipping_far_max = 200.0F;
constexpr auto camera_default_fov = 45.0F;
constexpr auto camera_speed = 0.1F;
constexpr auto camera_sensitivity = 0.1;

constexpr auto light_count = 10;
constexpr auto light_ambient = 0.2F;
constexpr auto light_default_diffuse = 1.0F;
constexpr auto light_directional_diffuse = 1.0F;
constexpr auto light_specular = 0.2F;
constexpr auto light_attenuation_constant = 1.0F;
constexpr auto light_attenuation_linear = 0.09F;
constexpr auto light_attenuation_quadratic = 0.032F;
constexpr auto light_cutoff_begin_min = 5.0F;
constexpr auto light_cutoff_begin = 25.0F;
constexpr auto light_cutoff_end = 35.0F;
constexpr auto light_cutoff_end_max = 120.0F;
constexpr std::array<glm::vec3, 3> light_positions = {glm::vec3(2.0F), glm::vec3(0.0F, 1.0F, 0.0F),
                                                      glm::vec3(-3.0F, 2.0F, -3.0F)};
constexpr std::array<glm::vec3, 3> light_directions = {glm::vec3(-1.0F, -2.0F, -2.0F), glm::vec3(0.0F, -1.0F, 0.0F),
                                                       glm::vec3()};

constexpr glm::vec3 material_default_ambient(0.2F);
constexpr auto material_default_shininess = 32.0F;
constexpr auto material_default_texture_mix = 0.8F;
constexpr glm::vec3 material_sphere_ambient(0.1F);
constexpr auto material_sphere_shininess = 32.0F;
constexpr glm::vec3 material_torus_ambient(1.0F);
constexpr auto material_torus_shininess = 2.0F;

constexpr transform_t object_cube_transforms = {glm::vec3(0.0F, 0.0F, -1.0F), 45.0F, glm::vec3(0.0F, 1.0F, 0.0F),
                                                glm::vec3(0.5F)};
constexpr transform_t object_plane_transforms = {glm::vec3(0.0F, -0.5F, 0.0F), 0.0F, glm::vec3(1.0F, 1.0F, 1.0F),
                                                 glm::vec3(10.0F)};
constexpr transform_t object_sphere_transforms = {glm::vec3(1.5F, 0.0F, -1.0F), 0.0F, glm::vec3(1.0F, 1.0F, 1.0F),
                                                  glm::vec3(0.6F)};
constexpr transform_t object_torus_transforms = {glm::vec3(-1.0F, 0.0F, -1.0F), 45.0F, glm::vec3(0.0F, 0.0F, 1.0F),
                                                 glm::vec3(0.6F)};
constexpr transform_t object_light_transforms = {glm::vec3(0.0F, 0.0F, 0.0F), 0.0F, glm::vec3(1.0F, 1.0F, 1.0F),
                                                 glm::vec3(0.1F)};

constexpr auto texture_layer_1 = 0;
constexpr auto texture_layer_2 = 1;
constexpr auto texture_diffuse = 2;
constexpr auto texture_specular = 3;

constexpr auto viewport_resolution_x = 1920;
constexpr auto viewport_resolution_y = 1080;
constexpr auto viewport_resolution_ratio = static_cast<float>(viewport_resolution_x) / viewport_resolution_y;
constexpr auto viewport_refresh_rate = 60;
constexpr glm::vec4 viewport_clear_color = {0.2F, 0.2F, 0.2F, 1.0F};

constexpr auto s_to_us_multiplier = 1000000.0;

} // namespace game_engine::configuration
