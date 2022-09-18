#pragma once

namespace game_engine::configuration {

constexpr auto texture_layer_1 = 0;
constexpr auto texture_layer_2 = 1;
constexpr auto texture_diffuse = 2;
constexpr auto texture_specular = 3;

constexpr auto light_ambient = 0.2F;
constexpr auto light_diffuse = 1.0F;
constexpr auto light_specular = 0.2F;
constexpr auto light_attenuation_constant = 1.0F;
constexpr auto light_attenuation_linear = 0.09F;
constexpr auto light_attenuation_quadratic = 0.032F;
constexpr auto light_cutoff_begin = 25.0F;
constexpr auto light_cutoff_end = 35.0F;

} // namespace game_engine::configuration
