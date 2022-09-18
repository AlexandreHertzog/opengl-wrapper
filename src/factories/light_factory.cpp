#include "light_factory.h"

#include "utils/configuration.h"

namespace game_engine {

light_factory_t::light_factory_t(opengl_cpp::gl_t &gl) : m_gl(gl) {
}

light_pointer_t light_factory_t::build_light(light_type_t type, glm::vec3 position, glm::vec3 direction) {
    light_pointer_t ret = std::make_unique<light_t>();
    switch (type) {
    case light_type_t::deactivated:
        return nullptr;
    case light_type_t::ambient:
        ret = std::make_unique<light_t>();
        break;
    case light_type_t::directional:
        ret = std::make_unique<directional_light_t>();
        break;
    case light_type_t::spot:
        ret = std::make_unique<spot_light_t>();
        break;
    }

    ret->m_position = std::move(position);
    ret->m_ambient = glm::vec3(configuration::light_ambient);
    ret->m_diffuse = glm::vec3(configuration::light_default_diffuse);
    ret->m_specular = glm::vec3(configuration::light_specular);
    ret->m_attenuation_constant = configuration::light_attenuation_constant;
    ret->m_attenuation_linear = configuration::light_attenuation_linear;
    ret->m_attenuation_quadratic = configuration::light_attenuation_quadratic;

    if (light_type_t::directional == type) {
        auto &direction_light = dynamic_cast<directional_light_t &>(*ret);
        direction_light.m_direction = std::move(direction);
        ret->m_attenuation_constant = 1.0F;
        ret->m_attenuation_linear = 0.0F;
        ret->m_attenuation_quadratic = 0.0F;
    } else if (light_type_t::spot == type) {
        auto &spot_light = dynamic_cast<spot_light_t &>(*ret);
        spot_light.m_direction = std::move(direction);
        spot_light.m_cutoff_begin = configuration::light_cutoff_begin;
        spot_light.m_cutoff_end = configuration::light_cutoff_end;
    }

    return ret;
}

} // namespace game_engine
