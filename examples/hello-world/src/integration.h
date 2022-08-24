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
    void init_callbacks();
    std::shared_ptr<opengl_wrapper::program> build_program();
    static opengl_wrapper::shape build_cube();
    void init_textures(opengl_wrapper::shape &s);
    void init_shapes(opengl_wrapper::shape &s);
    void prepare_render_loop();
    void render_loop();

  private:
    opengl_wrapper::window m_window;
    std::vector<opengl_wrapper::shape> m_shapes;
    const std::vector<glm::vec3> m_shape_positions;
    int m_shape_position_index{};
    opengl_wrapper::camera m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_xpos = 0.0;
    double m_last_cursor_ypos = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0;
};

} // namespace test_app
