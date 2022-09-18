#pragma once
#include "data_types/camera.h"
#include "data_types/shape.h"
#include "data_types/types.h"
#include "data_types/window.h"
#include "factories/light_factory.h"
#include "factories/program_factory.h"
#include "factories/shape_factory.h"
#include "factories/texture_factory.h"
#include "renderer.h"
#include <memory>
#include <opengl-cpp/backend/gl_impl.h>
#include <opengl-cpp/backend/glfw_impl.h>
#include <opengl-cpp/program.h>
#include <opengl-cpp/shader.h>
#include <opengl-cpp/texture.h>
#include <vector>

namespace game_engine {

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
    opengl_cpp::gl_impl_t m_gl;
    opengl_cpp::glfw_impl_t m_glfw;

    program_factory_t m_program_factory;
    texture_factory_t m_texture_factory;
    shape_factory_t m_shape_factory;
    light_factory_t m_light_factory;

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

    void shape_debug_ui(game_engine::shape_t &s);
    void update_shape_uniforms(opengl_cpp::program_t &p, game_engine::shape_t &s);
    std::string build_light_uniform_prefix(int i);
};

} // namespace game_engine
