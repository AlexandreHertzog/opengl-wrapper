#include "integration.h"

#include "data_types/image.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <boost/log/trivial.hpp>
#include <csignal>
#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>
#include <opengl-cpp/program.h>

using std::filesystem::path;

namespace game_engine {

integration_t::integration_t()
    : m_texture_factory(m_gl), m_shape_factory(m_gl, m_texture_factory),
      m_window(m_glfw, m_gl, configuration::viewport_resolution_x, configuration::viewport_resolution_y,
               "Test application"),
      m_renderer(m_gl),
      m_camera(configuration::camera_start_position, configuration::camera_start_front, configuration::camera_start_up),
      m_light_manager(m_gl), m_shape_manager(m_gl) {

    m_glfw.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    m_glfw.window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    m_glfw.window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

integration_t::~integration_t() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void integration_t::init_callbacks() { // NOLINT(readability-function-cognitive-complexity)
    m_window.set_framebuffer_callback([&](int width, int height) {
        m_renderer.set_viewport(width, height);
        render();
    });

    m_window.set_key_callback([&](int key, int action) {
        if (GLFW_PRESS != action && GLFW_REPEAT != action) {
            return;
        }
        switch (key) {
        case GLFW_KEY_ESCAPE:
            m_window.set_should_close(1);
            break;
        case GLFW_KEY_F12:
            m_wireframe = !m_wireframe;
            m_renderer.set_wireframe_mode(m_wireframe);
            break;
        case GLFW_KEY_F11:
            m_depth_test = !m_depth_test;
            m_renderer.set_depth_test(m_depth_test);
            break;
        case GLFW_KEY_F10:
            m_depth_view_enabled = !m_depth_view_enabled;
            break;
        case GLFW_KEY_F9:
            m_depth_view_debug = !m_depth_view_debug;
            break;
        case GLFW_KEY_SPACE:
            m_cursor_enabled = !m_cursor_enabled;
            m_window.set_input_mode(GLFW_CURSOR, m_cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            break;
        case GLFW_KEY_W:
            m_camera.step_front();
            break;
        case GLFW_KEY_S:
            m_camera.step_back();
            break;
        case GLFW_KEY_A:
            m_camera.step_left();
            break;
        case GLFW_KEY_D:
            m_camera.step_right();
            break;
        default:
            break;
        }
    });

    m_window.set_cursor_pos_callback([&](double position_x, double position_y) {
        if (m_first_cursor_iteration) {
            m_last_cursor_position_x = position_x;
            m_last_cursor_position_y = position_y;
            m_first_cursor_iteration = false;
        }

        const auto offset_position_x = (position_x - m_last_cursor_position_x) * configuration::camera_sensitivity;
        const auto offset_position_y = (m_last_cursor_position_y - position_y) * configuration::camera_sensitivity;

        m_last_cursor_position_x = position_x;
        m_last_cursor_position_y = position_y;

        m_yaw += offset_position_x;
        m_pitch = std::max(std::min(m_pitch + offset_position_y, configuration::camera_max_angle),
                           configuration::camera_min_angle);

        if (!m_cursor_enabled) {
            m_camera.set_front(m_pitch, m_yaw);
        }
    });
}

void integration_t::build_shapes() {
    m_shape_manager.add_object_shape(m_shape_factory.build_cube());
    m_shape_manager.add_object_shape(m_shape_factory.build_plane());
    m_shape_manager.add_object_shape(m_shape_factory.build_sphere());
    m_shape_manager.add_object_shape(m_shape_factory.build_torus());

    auto light_texture = m_texture_factory.build_white_texture();

    auto *light0 = m_light_manager.add_light<spot_light_t>();
    assert(nullptr != light0);
    light0->m_position = configuration::light_positions[0];
    light0->m_direction = configuration::light_directions[0];
    light0->m_shape = m_shape_factory.build_light_shape();
    m_shape_manager.add_light_shape(light0->m_shape);

    auto *light1 = m_light_manager.add_light<directional_light_t>();
    assert(nullptr != light1);
    light1->m_position = configuration::light_positions[1];
    light1->m_direction = configuration::light_directions[1];
    light1->m_shape = m_shape_factory.build_light_shape();
    m_shape_manager.add_light_shape(light1->m_shape);

    auto *light2 = m_light_manager.add_light<directional_light_t>();
    assert(nullptr != light2);
    light2->m_position = configuration::light_positions[2];
    light2->m_direction = configuration::light_directions[2];
    light2->m_diffuse = glm::vec3(configuration::light_directional_diffuse);
    light2->m_shape = m_shape_factory.build_light_shape();
    m_shape_manager.add_light_shape(light2->m_shape);
}

void integration_t::render_loop() {
    m_renderer.set_depth_test(m_depth_test);
    m_renderer.set_clear_color(configuration::viewport_clear_color);

    for (auto &program_shape : m_shape_manager) {
        assert(program_shape.second);
        program_shape.second->load_vertices();
    }

    while (!m_window.get_should_close()) {
        render();
    }
}
void integration_t::render() {
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    auto frame_time_us = configuration::s_to_us_multiplier / configuration::viewport_refresh_rate;
    auto start_time = high_resolution_clock::now();

    build_ui();

    m_renderer.clear();

    for (auto &program_shape : m_shape_manager) {
        assert(program_shape.first);
        program_shape.first->use();

        update_projection_uniforms(*program_shape.first);
        m_light_manager.update_light_uniforms(*program_shape.first);
        update_parameter_uniforms(*program_shape.first);

        assert(program_shape.second);
        update_shape_uniforms(*program_shape.first, *program_shape.second);
        m_renderer.draw(*program_shape.second);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.poll_events();

    duration<double, std::micro> loop_time_us = high_resolution_clock::now() - start_time;

    if (loop_time_us.count() < frame_time_us) {
        const auto wait_time_us = frame_time_us - loop_time_us.count();
        BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
        usleep(static_cast<unsigned int>(wait_time_us));
    } else {
        BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
    }

    m_window.swap_buffers();
}

void integration_t::build_ui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("OpenGL Wrapper test app");

    if (ImGui::CollapsingHeader("Depth parameters")) {
        ImGui::Checkbox("Enabled", &m_depth_view_enabled);
        ImGui::Checkbox("Debug", &m_depth_view_debug);
        ImGui::SliderFloat("Near", &m_depth_near, configuration::camera_clipping_near, m_depth_far);
        ImGui::SliderFloat("Far", &m_depth_far, m_depth_near, configuration::camera_clipping_far_max);
    }

    int i = 0;
    for (auto &light : m_light_manager) {
        if (!light) {
            continue;
        }

        const std::string name = "Light " + std::to_string(i++);
        if (ImGui::CollapsingHeader(name.c_str())) {

            ImGui::InputScalarN("Position", ImGuiDataType_Float, &light->m_position, 3);

            auto *directional_light = dynamic_cast<directional_light_t *>(light.get());
            if (nullptr != directional_light) {
                ImGui::InputScalarN("Direction", ImGuiDataType_Float, &directional_light->m_direction, 3);
            } else {
                auto *spot_light = dynamic_cast<spot_light_t *>(light.get());
                if (nullptr != spot_light) {
                    ImGui::InputScalarN("Direction", ImGuiDataType_Float, &spot_light->m_direction, 3);
                    ImGui::SliderFloat("Cutoff begin", &spot_light->m_cutoff_begin,
                                       configuration::light_cutoff_begin_min, spot_light->m_cutoff_end);
                    ImGui::SliderFloat("Cutoff end", &spot_light->m_cutoff_end, spot_light->m_cutoff_begin,
                                       configuration::light_cutoff_end_max);
                }
            }

            const float min_material = 0.0F;
            const float max_material = 2.0F;
            ImGui::SliderScalarN("Ambient", ImGuiDataType_Float, &light->m_ambient, 3, &min_material, &max_material);
            ImGui::SliderScalarN("Diffuse", ImGuiDataType_Float, &light->m_diffuse, 3, &min_material, &max_material);
            ImGui::SliderScalarN("Specular", ImGuiDataType_Float, &light->m_specular, 3, &min_material, &max_material);
            ImGui::InputFloat("Constant attenuation", &light->m_attenuation_constant);
            ImGui::InputFloat("Linear attenuation", &light->m_attenuation_linear);
            ImGui::InputFloat("Linear quadratic", &light->m_attenuation_quadratic);
        }
    }

    for (auto &program_shape : m_shape_manager) {
        assert(program_shape.second);
        if (ImGui::CollapsingHeader(program_shape.second->get_mesh().get_name().c_str())) {
            shape_debug_ui(*program_shape.second);
        }
    }

    ImGui::End();

    ImGui::Render();
}

void integration_t::update_projection_uniforms(opengl_cpp::program_t &p) {
    auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
    p.set_uniform("uniform_view", view);

    auto projection =
        glm::perspective(glm::radians(configuration::camera_default_fov), configuration::viewport_resolution_ratio,
                         configuration::camera_clipping_near, configuration::camera_clipping_far);
    p.set_uniform("uniform_projection", projection);
}

void integration_t::update_parameter_uniforms(opengl_cpp::program_t &p) const {
    p.set_uniform("uniform_depth.debug", m_depth_view_debug);
    p.set_uniform("uniform_depth.enabled", m_depth_view_enabled);
    p.set_uniform("uniform_depth.near", m_depth_near);
    p.set_uniform("uniform_depth.far", m_depth_far);
}

void integration_t::update_shape_uniforms(opengl_cpp::program_t &p, shape_t &s) {
    p.set_uniform("uniform_model", s.model_transformations());
    p.set_uniform("uniform_material.has_diffuse", static_cast<bool>(s.get_material().m_diffuse));
    p.set_uniform("uniform_material.has_specular", static_cast<bool>(s.get_material().m_specular));
    p.set_uniform("uniform_material.ambient", s.get_material().m_ambient);
    p.set_uniform("uniform_material.shininess", s.get_material().m_shininess);
    p.set_uniform("uniform_material.texture1", configuration::texture_layer_1);
    p.set_uniform("uniform_material.texture2", configuration::texture_layer_2);
    p.set_uniform("uniform_material.diffuse", configuration::texture_diffuse);
    p.set_uniform("uniform_material.specular", configuration::texture_specular);
    p.set_uniform("uniform_material.texture_mix", s.get_material().m_texture_mix);
}

void integration_t::shape_debug_ui(shape_t &s) {
    const auto min_translation = -10.0F;
    const auto max_translation = 10.0F;
    const auto min_rotation_angle = -180.0F;
    const auto max_rotation_angle = 180.0F;
    const auto min_shininess = 0.0F;
    const auto max_shininess = 64.0F;
    const float min_material = 0.0F;
    const float max_material = 2.0F;

    ImGui::SliderScalarN("Position", ImGuiDataType_Float, &s.get_transform().m_translation, 3, &min_translation,
                         &max_translation);

    ImGui::SliderFloat("Rot angle", &s.get_transform().m_rotation_angle, min_rotation_angle, max_rotation_angle);
    ImGui::InputScalarN("Rot axis", ImGuiDataType_Float, &s.get_transform().m_rotation_axis, 3);
    ImGui::InputScalarN("Scale", ImGuiDataType_Float, &s.get_transform().m_scale, 3);
    ImGui::SliderScalarN("Ambient", ImGuiDataType_Float, &s.get_material().m_ambient, 3, &min_material, &max_material);
    ImGui::SliderFloat("Shininess", &s.get_material().m_shininess, min_shininess, max_shininess);

    if (s.get_material().m_diffuse) {
        ImGui::BeginDisabled(true);
    }
    ImGui::SliderScalarN("Diffuse", ImGuiDataType_Float, &s.get_material().m_diffuse, 3, &min_material, &max_material);
    if (s.get_material().m_diffuse) {
        ImGui::EndDisabled();
    }

    if (s.get_material().m_specular) {
        ImGui::BeginDisabled(true);
    }

    ImGui::SliderScalarN("Specular", ImGuiDataType_Float, &s.get_material().m_specular, 3, &min_material,
                         &max_material);

    if (s.get_material().m_specular) {
        ImGui::EndDisabled();
    }
}

} // namespace game_engine