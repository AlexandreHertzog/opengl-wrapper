#pragma once
#include "opengl-wrapper/data_types/camera.h"
#include "opengl-wrapper/data_types/program.h"
#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/data_types/window.h"
#include <memory>
#include <vector>

namespace test_app {

constexpr auto light_count = 10;

class integration_t {
  public:
    integration_t();
    ~integration_t();
    integration_t(const integration_t &) = delete;
    integration_t(integration_t &&) = delete;
    integration_t &operator=(const integration_t &) = delete;
    integration_t &operator=(integration_t &&) = delete;

    void init_callbacks();
    void build_shapes();
    void render_loop();

  private:
    using shape_pointer_t = std::shared_ptr<opengl_wrapper::shape_t>;
    using shape_vector_t = std::vector<shape_pointer_t>;
    using program_pointer_t = std::shared_ptr<opengl_wrapper::program_t>;
    using light_pointer_t = std::shared_ptr<opengl_wrapper::light_t>;

    enum class light_type_t {
        deactivated = 0,
        ambient = 1,
        directional = 2,
        spot = 3
    };

    opengl_wrapper::window_t m_window;
    std::map<program_pointer_t, shape_vector_t> m_program_shape_map;
    std::array<light_pointer_t, light_count> m_lights;
    std::map<light_pointer_t, shape_pointer_t> m_light_shapes;
    opengl_wrapper::camera_t m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{true};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_position_x = 0.0;
    double m_last_cursor_position_y = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)

    void build_ui();
    void update_light_uniforms(opengl_wrapper::program_t &p);
    void update_projection_uniforms(opengl_wrapper::program_t &p);
    void render();

    static program_pointer_t build_object_program();
    static program_pointer_t build_light_program();
    static shape_pointer_t build_cube(opengl_wrapper::texture_t::pointer_t &base_texture);
    static shape_pointer_t build_plane(opengl_wrapper::texture_t::pointer_t &base_texture);
    static shape_pointer_t build_sphere(opengl_wrapper::texture_t::pointer_t &base_texture);
    static shape_pointer_t build_torus(opengl_wrapper::texture_t::pointer_t &base_texture);
    shape_pointer_t build_light_shape(const light_pointer_t &light, opengl_wrapper::texture_t::pointer_t &base_texture);
    static integration_t::light_pointer_t build_light(light_type_t type, glm::vec3 position, glm::vec3 direction);
    static void shape_debug_ui(opengl_wrapper::shape_t &s);
    static void update_shape_uniforms(opengl_wrapper::program_t &p, opengl_wrapper::shape_t &s);
    static std::string build_light_uniform_prefix(int i);
};

} // namespace test_app
