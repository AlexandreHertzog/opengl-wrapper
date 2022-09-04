#include "graphics.h"
#include "ogl_graphics.h"

namespace opengl_wrapper {

graphics_t *graphics_t::m_instance; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

graphics_t &graphics_t::instance() {
    if (nullptr == m_instance) {
        static ogl_graphics_t default_graphics;
        m_instance = &default_graphics;
    }
    return *m_instance;
}

void graphics_t::set_instance(opengl_wrapper::graphics_t *instance) {
    m_instance = instance;
}

} // namespace opengl_wrapper
