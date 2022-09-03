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

constexpr auto LIGHT_COUNT = 10;

class integration {
  public:
    integration();
    ~integration();
    integration(const integration &) = delete;
    integration(integration &&) = delete;
    integration &operator=(const integration &) = delete;
    integration &operator=(integration &&) = delete;

    void init_callbacks();
    void build_shapes();
    void render_loop();

  private:
    using shape_vector_t = std::vector<opengl_wrapper::shape>;
    using program_pointer_t = std::shared_ptr<opengl_wrapper::program>;
    using light_pointer_t = std::unique_ptr<opengl_wrapper::light>;

    enum class light_type_t {
        deactivated = 0,
        ambient = 1,
        directional = 2,
        spot = 3
    };

    opengl_wrapper::window m_window;
    std::map<program_pointer_t, shape_vector_t> m_program_shape_map;
    std::array<light_pointer_t, LIGHT_COUNT> m_lights;
    opengl_wrapper::camera m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{true};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_position_x = 0.0;
    double m_last_cursor_position_y = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0;

    void build_ui();
    void update_light_uniforms(opengl_wrapper::program &p);
    void update_projection_uniforms(opengl_wrapper::program &p);
    void render();

    static program_pointer_t build_object_program();
    static program_pointer_t build_light_program();
    static opengl_wrapper::shape build_cube(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_plane(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_sphere(opengl_wrapper::texture::pointer_t &base_texture);
    static opengl_wrapper::shape build_torus(opengl_wrapper::texture::pointer_t &base_texture);
    static integration::light_pointer_t build_light(light_type_t type, glm::vec3 position, glm::vec3 direction);
    static void shape_debug_ui(opengl_wrapper::shape &s);
    static void update_shape_uniforms(opengl_wrapper::program &p, opengl_wrapper::shape &s);
    static std::string build_light_uniform_prefix(int i);
};

} // namespace test_app
