#pragma once
#include "opengl-wrapper/data_types/camera.h"
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
    std::shared_ptr<opengl_wrapper::program> build_object_program();
    std::shared_ptr<opengl_wrapper::program> build_light_program();
    void build_shapes();
    void prepare_render_loop();
    void render_loop();

  private:
    opengl_wrapper::window m_window;
    std::vector<opengl_wrapper::shape> m_shapes;
    std::vector<opengl_wrapper::shape> m_lights;
    opengl_wrapper::camera m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{true};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_xpos = 0.0;
    double m_last_cursor_ypos = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0;

    bool m_auto_rotate_light{true};

    const std::function<void(opengl_wrapper::program &, opengl_wrapper::shape &)> m_default_callback;

    void build_ui();

    static opengl_wrapper::shape build_cube(std::shared_ptr<opengl_wrapper::program> &object_program,
                                            opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_plane(std::shared_ptr<opengl_wrapper::program> &object_program,
                                             opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_sphere(std::shared_ptr<opengl_wrapper::program> &object_program,
                                              opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_torus(std::shared_ptr<opengl_wrapper::program> &object_program,
                                             opengl_wrapper::texture::pointer_t &base_texture);
    opengl_wrapper::shape build_light(std::shared_ptr<opengl_wrapper::program> &light_program);
    void shape_debug_ui(opengl_wrapper::shape &s);
};

} // namespace test_app
