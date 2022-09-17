#include "factories/program_factory.h"

#include <filesystem>
#include <opengl-cpp/shader.h>

namespace game_engine {

using std::filesystem::path;

program_factory_t::program_factory_t(opengl_cpp::gl_t &gl) : m_gl(gl) {
}

program_pointer_t program_factory_t::build_object_program() {
    return build_program("shaders/object.vert", "shaders/object.frag");
}

program_pointer_t program_factory_t::build_light_program() {
    return build_program("shaders/light.vert", "shaders/light.frag");
}

program_pointer_t program_factory_t::build_program(const char *vert_source_path, const char *frag_source_path) {
    using opengl_cpp::shader_t;
    using opengl_cpp::shader_type_t;

    auto ret = std::make_shared<opengl_cpp::program_t>(m_gl);
    ret->add_shader(shader_t(m_gl, shader_type_t::vertex, path(vert_source_path)));
    ret->add_shader(shader_t(m_gl, shader_type_t::fragment, path(frag_source_path)));
    ret->link();
    return ret;
}

} // namespace game_engine