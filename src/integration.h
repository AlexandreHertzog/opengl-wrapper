#pragma once
#include "game-engine/data_types/camera.h"
#include "game-engine/data_types/shape.h"
#include "game-engine/data_types/window.h"
#include "opengl-cpp/backend/gl_impl.h"
#include "opengl-cpp/backend/glfw_impl.h"
#include "renderer.h"
#include <memory>
#include <opengl-cpp/program.h>
#include <opengl-cpp/shader.h>
#include <opengl-cpp/texture.h>
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
    using light_pointer_t = std::shared_ptr<game_engine::light_t>;
    using shape_pointer_t = std::shared_ptr<game_engine::shape_t>;
    using shape_vector_t = std::vector<shape_pointer_t>;
    using program_pointer_t = std::shared_ptr<opengl_cpp::program_t>;
    using texture_pointer_t = std::shared_ptr<opengl_cpp::texture_t>;

    enum class light_type_t {
        deactivated = 0,
        ambient = 1,
        directional = 2,
        spot = 3
    };

    opengl_cpp::gl_impl_t m_gl;
    opengl_cpp::glfw_impl_t m_glfw;

    game_engine::window_t m_window;
    game_engine::renderer_t m_renderer;
    std::map<program_pointer_t, shape_vector_t> m_program_shape_map;
    std::array<light_pointer_t, light_count> m_lights;
    std::map<light_pointer_t, shape_pointer_t> m_light_shapes;
    game_engine::camera_t m_camera;

    bool m_wireframe{};
    bool m_cursor_enabled{true};
    bool m_depth_test{true};
    bool m_depth_view_enabled{true};
    bool m_depth_view_debug{false};
    float m_depth_near{0.1F};
    float m_depth_far{100.0F};
    bool m_first_cursor_iteration = true;
    double m_last_cursor_position_x = 0.0;
    double m_last_cursor_position_y = 0.0;
    double m_pitch = 0.0;
    double m_yaw = -90.0; // NOLINT(cppcoreguidelines-avoid-magic-numbers)

    void build_ui();
    void update_light_uniforms(opengl_cpp::program_t &p);
    void update_projection_uniforms(opengl_cpp::program_t &p);
    void update_parameter_uniforms(opengl_cpp::program_t &p) const;
    void render();

    texture_pointer_t build_texture(const char *path, int texture_layer);
    program_pointer_t build_object_program();
    program_pointer_t build_light_program();
    shape_pointer_t build_cube(texture_pointer_t &base_texture);
    shape_pointer_t build_plane(texture_pointer_t &base_texture);
    shape_pointer_t build_sphere(texture_pointer_t &base_texture);
    shape_pointer_t build_torus(texture_pointer_t &base_texture);
    shape_pointer_t build_light_shape(const light_pointer_t &light, texture_pointer_t &base_texture);
    integration_t::light_pointer_t build_light(light_type_t type, glm::vec3 position, glm::vec3 direction);
    void shape_debug_ui(game_engine::shape_t &s);
    void update_shape_uniforms(opengl_cpp::program_t &p, game_engine::shape_t &s);
    std::string build_light_uniform_prefix(int i);
};

} // namespace test_app
