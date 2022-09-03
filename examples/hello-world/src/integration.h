#pragma once
#include "opengl-wrapper/data_types/camera.h"
#include "opengl-wrapper/data_types/light.h"
#include "opengl-wrapper/data_types/program.h"
#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/data_types/window.h"
#include <memory>
#include <vector>

namespace test_app {

class integration {
  public:
    integration();
    ~integration();
    void init_callbacks();
    void build_shapes();
    void render_loop();

  private:
    using shape_vector_t = std::vector<opengl_wrapper::shape>;
    using program_pointer_t = std::shared_ptr<opengl_wrapper::program>;

    enum class light_type_t {
        ambient = 0,
        directional = 1,
        spot = 2
    };

    opengl_wrapper::window m_window;
    std::map<program_pointer_t, shape_vector_t> m_program_shape_map;
    std::unique_ptr<opengl_wrapper::light> m_light;
    opengl_wrapper::camera m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{true};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_xpos = 0.0;
    double m_last_cursor_ypos = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0;

    void build_ui();
    program_pointer_t build_object_program();
    program_pointer_t build_light_program();
    void update_light_uniforms(opengl_wrapper::program &p);
    void update_projection_uniforms(opengl_wrapper::program &p);
    void render();

    static opengl_wrapper::shape build_cube(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_plane(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_sphere(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_torus(opengl_wrapper::texture::pointer_t &base_texture);
    static std::unique_ptr<opengl_wrapper::light> build_light(light_type_t type);
    static void shape_debug_ui(opengl_wrapper::shape &s);
    static void update_shape_uniforms(opengl_wrapper::program &p, opengl_wrapper::shape &s);
};

} // namespace test_app
