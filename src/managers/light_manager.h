#pragma once

#include "factories/light_factory.h"
#include "utils/configuration.h"

namespace game_engine {

class light_manager_t {
  public:
    using vector_t = std::vector<light_pointer_t>;

    light_manager_t(opengl_cpp::gl_t &gl);

    void update_light_uniforms(opengl_cpp::program_t &p);
    vector_t::iterator begin();
    vector_t::iterator end();

    template <class light_template_t> light_template_t *add_light() {
        if (m_values.size() >= configuration::light_count) {
            return nullptr;
        }

        auto ret = m_values.emplace_back(m_light_factory.build_light(light_template_t::m_type));
        return dynamic_cast<light_template_t *>(ret.get());
    }

  private:
    opengl_cpp::gl_t &m_gl;
    vector_t m_values;
    light_factory_t m_light_factory;

    std::string build_light_uniform_prefix(int i);
};

} // namespace game_engine
