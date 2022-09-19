#include "light_manager.h"

#include "data_types/shape.h"
#include <opengl-cpp/program.h>

namespace game_engine {

light_manager_t::light_manager_t(opengl_cpp::gl_t &gl) : m_gl(gl), m_light_factory(m_gl) {
}

void light_manager_t::update_light_uniforms(opengl_cpp::program_t &p) {
    int i = 0;
    for (auto &light : m_values) {
        if (light->m_shape) {
            light->m_shape->get_transform().m_translation = light->m_position;
        }

        const auto prefix = build_light_uniform_prefix(i++);
        if (!light) {
            p.set_uniform(prefix + "type", static_cast<int>(light_type_t::deactivated));
            continue;
        }

        p.set_uniform(prefix + "position", light->m_position);
        p.set_uniform(prefix + "ambient", light->m_ambient);
        p.set_uniform(prefix + "diffuse", light->m_diffuse);
        p.set_uniform(prefix + "specular", light->m_specular);
        p.set_uniform(prefix + "attenuation_constant", light->m_attenuation_constant);
        p.set_uniform(prefix + "attenuation_linear", light->m_attenuation_linear);
        p.set_uniform(prefix + "attenuation_quadratic", light->m_attenuation_quadratic);

        auto *direction_light = dynamic_cast<directional_light_t *>(light.get());
        if (nullptr != direction_light) {
            p.set_uniform(prefix + "type", static_cast<int>(light_type_t::directional));
            p.set_uniform(prefix + "direction", direction_light->m_direction);
        } else {
            auto *spot_light = dynamic_cast<spot_light_t *>(light.get());
            if (nullptr != spot_light) {
                p.set_uniform(prefix + "type", static_cast<int>(light_type_t::spot));
                p.set_uniform(prefix + "direction", spot_light->m_direction);
                p.set_uniform(prefix + "cutoff_begin", glm::cos(glm::radians(spot_light->m_cutoff_begin)));
                p.set_uniform(prefix + "cutoff_end", glm::cos(glm::radians(spot_light->m_cutoff_end)));
            } else {
                p.set_uniform(prefix + "type", static_cast<int>(light_type_t::ambient));
            }
        }
    }
}

light_manager_t::vector_t::iterator light_manager_t::begin() {
    return m_values.begin();
}

light_manager_t::vector_t::iterator light_manager_t::end() {
    return m_values.end();
}

std::string light_manager_t::build_light_uniform_prefix(int i) {
    return "uniform_light[" + std::to_string(i) + "].";
}

} // namespace game_engine
