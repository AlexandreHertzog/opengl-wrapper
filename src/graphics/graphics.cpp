#include "graphics.h"
#include "ogl_graphics.h"

namespace opengl_wrapper {

graphics *graphics::m_instance;

graphics &graphics::instance() {
    if (!m_instance) {
        static ogl_graphics default_graphics;
        m_instance = &default_graphics;
    }
    return *m_instance;
}

void graphics::set_instance(opengl_wrapper::graphics *instance) {
    m_instance = instance;
}

}