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

namespace {

constexpr auto texture_layer_1 = 0;
constexpr auto texture_layer_2 = 1;
constexpr auto texture_diffuse = 2;
constexpr auto texture_specular = 3;

constexpr auto resolution_x = 1920;
constexpr auto resolution_y = 1080;
constexpr auto resolution_ratio = static_cast<float>(resolution_x) / resolution_y;
constexpr auto default_fov = 45.0F;

constexpr glm::vec3 default_camera_pos = {0.0, 3.0, 8.0};
constexpr glm::vec3 default_camera_front = {0.0, -0.3, -1.0};
constexpr glm::vec3 default_camera_up = {0.0, 1.0, 0.0};
constexpr auto camera_max_angle = 89.0;
constexpr auto camera_min_angle = -89.0;
constexpr auto clipping_near = 0.1F;
constexpr auto clipping_far = 100.0F;
constexpr auto refresh_rate = 60;

constexpr auto default_ambient = 0.2F;
constexpr auto default_shininess = 32.0F;
constexpr auto default_texture_mix = 0.8F;

} // namespace

namespace game_engine {

integration_t::integration_t()
    : m_window(m_glfw, m_gl, resolution_x, resolution_y, "Test application"), m_renderer(m_gl),
      m_camera(default_camera_pos, default_camera_front, default_camera_up) {

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
    constexpr auto camera_speed = 0.1F;

    m_window.set_framebuffer_callback([&](int width, int height) {
        m_renderer.set_viewport(width, height);
        render();
    });

    m_window.set_key_callback([&](int key, int action) {
        if (GLFW_PRESS == action && GLFW_KEY_ESCAPE == key) {
            m_window.set_should_close(1);
        }
        if (GLFW_PRESS == action && GLFW_KEY_F12 == key) {
            m_wireframe = !m_wireframe;
            m_renderer.set_wireframe_mode(m_wireframe);
        }
        if (GLFW_PRESS == action && GLFW_KEY_F11 == key) {
            m_depth_test = !m_depth_test;
            m_renderer.set_depth_test(m_depth_test);
        }
        if (GLFW_PRESS == action && GLFW_KEY_F10 == key) {
            m_depth_view_enabled = !m_depth_view_enabled;
        }
        if (GLFW_PRESS == action && GLFW_KEY_F9 == key) {
            m_depth_view_debug = !m_depth_view_debug;
        }
        if (GLFW_PRESS == action && GLFW_KEY_SPACE == key) {
            m_cursor_enabled = !m_cursor_enabled;
            m_window.set_input_mode(GLFW_CURSOR, m_cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_W == key) {
            m_camera.set_position(m_camera.get_position() + camera_speed * m_camera.get_front());
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_S == key) {
            m_camera.set_position(m_camera.get_position() - camera_speed * m_camera.get_front());
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_A == key) {
            m_camera.set_position(m_camera.get_position() -
                                  camera_speed * glm::normalize(glm::cross(m_camera.get_front(), m_camera.get_up())));
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_D == key) {
            m_camera.set_position(m_camera.get_position() +
                                  camera_speed * glm::normalize(glm::cross(m_camera.get_front(), m_camera.get_up())));
        }
    });

    m_window.set_cursor_pos_callback([&](double position_x, double position_y) {
        if (m_first_cursor_iteration) {
            m_last_cursor_position_x = position_x;
            m_last_cursor_position_y = position_y;
            m_first_cursor_iteration = false;
        }

        constexpr auto sensitivity = 0.1;
        const auto offset_position_x = (position_x - m_last_cursor_position_x) * sensitivity;
        const auto offset_position_y = (m_last_cursor_position_y - position_y) * sensitivity;

        m_last_cursor_position_x = position_x;
        m_last_cursor_position_y = position_y;

        m_yaw += offset_position_x;
        m_pitch = std::max(std::min(m_pitch + offset_position_y, camera_max_angle), camera_min_angle);

        if (!m_cursor_enabled) {
            m_camera.set_front(m_pitch, m_yaw);
        }
    });
}

void integration_t::build_shapes() {
    auto object_program = build_object_program();
    auto light_program = build_light_program();
    auto base_texture = build_texture("./textures/checker.png", texture_layer_1);

    m_program_shape_map[object_program].emplace_back(build_cube(base_texture));
    m_program_shape_map[object_program].emplace_back(build_plane(base_texture));
    m_program_shape_map[object_program].emplace_back(build_sphere(base_texture));
    m_program_shape_map[object_program].emplace_back(build_torus(base_texture));

    constexpr glm::vec3 light0_pos = glm::vec3(2.0F);
    constexpr glm::vec3 light0_dir = glm::vec3(-1.0F, -2.0F, -2.0F);
    constexpr glm::vec3 light1_pos = glm::vec3(0.0F, 1.0F, 0.0F);
    constexpr glm::vec3 light1_dir = glm::vec3(0.0F, -1.0F, 0.0F);
    constexpr glm::vec3 light2_pos = glm::vec3(-3.0F, 2.0F, -3.0F);
    constexpr auto diffuse = 0.2F;

    auto light_texture = build_texture("./textures/white.png", texture_layer_1);

    m_lights[0] = build_light(light_type_t::spot, light0_pos, light0_dir);
    m_program_shape_map[light_program].emplace_back(build_light_shape(m_lights[0], light_texture));

    m_lights[1] = build_light(light_type_t::directional, light1_pos, light1_dir);
    m_lights[1]->m_diffuse = glm::vec3(diffuse);
    m_program_shape_map[light_program].emplace_back(build_light_shape(m_lights[1], light_texture));

    m_lights[2] = build_light(light_type_t::directional, light2_pos, glm::vec3());
    m_lights[2]->m_diffuse = glm::vec3(diffuse);
    m_program_shape_map[light_program].emplace_back(build_light_shape(m_lights[2], light_texture));
}

void integration_t::render_loop() {
    constexpr glm::vec4 clear_color = {0.2F, 0.2F, 0.2F, 1.0F};

    m_renderer.set_depth_test(m_depth_test);
    m_renderer.set_clear_color(clear_color);

    for (auto &program_shapes : m_program_shape_map) {
        for (auto &shape : program_shapes.second) {
            assert(shape);
            shape->load_vertices();
        }
    }

    while (!m_window.get_should_close()) {
        render();
    }
}
void integration_t::render() {
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    constexpr auto s_to_us_multiplier = 1000000.0;
    auto frame_time_us = s_to_us_multiplier / refresh_rate;

    auto start_time = high_resolution_clock::now();

    build_ui();

    m_renderer.clear();

    for (auto &program_shape : m_program_shape_map) {
        assert(program_shape.first);
        program_shape.first->use();

        update_projection_uniforms(*program_shape.first);
        update_light_uniforms(*program_shape.first);
        update_parameter_uniforms(*program_shape.first);

        for (auto &shape : program_shape.second) {
            assert(shape);
            update_shape_uniforms(*program_shape.first, *shape);
            m_renderer.draw(*shape);
        }
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

    //    ImGui::ShowDemoWindow();

    ImGui::Begin("OpenGL Wrapper test app");

    if (ImGui::CollapsingHeader("Depth parameters")) {
        ImGui::Checkbox("Enabled", &m_depth_view_enabled);
        ImGui::Checkbox("Debug", &m_depth_view_debug);
        ImGui::SliderFloat("Near", &m_depth_near, 0.1, m_depth_far);
        ImGui::SliderFloat("Far", &m_depth_far, m_depth_near, 200.0);
    }

    int i = 0;
    for (auto &light : m_lights) {
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
                    constexpr auto min_cutoff_begin = 5.0F;
                    constexpr auto max_cutoff_end = 120.0F;

                    ImGui::InputScalarN("Direction", ImGuiDataType_Float, &spot_light->m_direction, 3);
                    ImGui::SliderFloat("Cutoff begin", &spot_light->m_cutoff_begin, min_cutoff_begin,
                                       spot_light->m_cutoff_end);
                    ImGui::SliderFloat("Cutoff end", &spot_light->m_cutoff_end, spot_light->m_cutoff_begin,
                                       max_cutoff_end);
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

    for (auto &program_shapes : m_program_shape_map) {
        for (auto &shape : program_shapes.second) {
            assert(shape);
            if (ImGui::CollapsingHeader(shape->get_mesh().get_name().c_str())) {
                shape_debug_ui(*shape);
            }
        }
    }

    ImGui::End();

    ImGui::Render();
}

void integration_t::update_light_uniforms(opengl_cpp::program_t &p) {
    int i = 0;
    for (auto &light : m_lights) {
        const auto prefix = build_light_uniform_prefix(i++);
        if (!light) {
            p.set_uniform(prefix + "type", static_cast<int>(light_type_t::deactivated));
            continue;
        }

        p.set_uniform(prefix + "position", light->m_position);
        p.set_uniform(prefix + "ambient", light->m_ambient);
        p.set_uniform(prefix + "diffuse", light->m_diffuse);
        p.set_uniform(prefix + "specular", light->m_specular);
        p.set_uniform(prefix + "attenuation_constant", light->m_attenuation_constant);
        p.set_uniform(prefix + "attenuation_linear", light->m_attenuation_linear);
        p.set_uniform(prefix + "attenuation_quadratic", light->m_attenuation_quadratic);

        auto *direction_light = dynamic_cast<directional_light_t *>(light.get());
        if (nullptr != direction_light) {
            p.set_uniform(prefix + "type", static_cast<int>(light_type_t::directional));
            p.set_uniform(prefix + "direction", direction_light->m_direction);
        } else {
            auto *spot_light = dynamic_cast<spot_light_t *>(light.get());
            if (nullptr != spot_light) {
                p.set_uniform(prefix + "type", static_cast<int>(light_type_t::spot));
                p.set_uniform(prefix + "direction", spot_light->m_direction);
                p.set_uniform(prefix + "cutoff_begin", glm::cos(glm::radians(spot_light->m_cutoff_begin)));
                p.set_uniform(prefix + "cutoff_end", glm::cos(glm::radians(spot_light->m_cutoff_end)));
            } else {
                p.set_uniform(prefix + "type", static_cast<int>(light_type_t::ambient));
            }
        }
    }
}

std::string integration_t::build_light_uniform_prefix(int i) {
    return "uniform_light[" + std::to_string(i) + "].";
}

void integration_t::update_projection_uniforms(opengl_cpp::program_t &p) {
    auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
    p.set_uniform("uniform_view", view);

    auto projection = glm::perspective(glm::radians(default_fov), resolution_ratio, clipping_near, clipping_far);
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
    p.set_uniform("uniform_material.texture1", texture_layer_1);
    p.set_uniform("uniform_material.texture2", texture_layer_2);
    p.set_uniform("uniform_material.diffuse", texture_diffuse);
    p.set_uniform("uniform_material.specular", texture_specular);
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

texture_pointer_t integration_t::build_texture(const char *path, int texture_layer) {
    using opengl_cpp::texture_format_t;
    using opengl_cpp::texture_parameter_t;
    using opengl_cpp::texture_parameter_values_t;
    using opengl_cpp::texture_target_t;

    auto ret = std::make_shared<opengl_cpp::texture_t>(m_gl, texture_layer, texture_target_t::tex_2d);
    ret->bind();

    ret->set_parameter(texture_parameter_t::wrap_s, texture_parameter_values_t::repeat);
    ret->set_parameter(texture_parameter_t::wrap_t, texture_parameter_values_t::repeat);
    ret->set_parameter(texture_parameter_t::min_filter, texture_parameter_values_t::linear_mipmap_linear);
    ret->set_parameter(texture_parameter_t::mag_filter, texture_parameter_values_t::linear);

    image_t image(path);
    ret->set_image(image.get_width(), image.get_height(),
                   image.has_alpha() ? texture_format_t::rgba : texture_format_t::rgb, image.get_data());
    ret->generate_mipmap();
    return ret;
}

std::shared_ptr<opengl_cpp::program_t> integration_t::build_object_program() {
    using opengl_cpp::shader_t;
    using opengl_cpp::shader_type_t;

    auto ret = std::make_shared<opengl_cpp::program_t>(m_gl);
    ret->add_shader(shader_t(m_gl, shader_type_t::vertex, path("shaders/object.vert")));
    ret->add_shader(shader_t(m_gl, shader_type_t::fragment, path("shaders/object.frag")));
    ret->link();
    return ret;
}

std::shared_ptr<opengl_cpp::program_t> integration_t::build_light_program() {
    using opengl_cpp::shader_t;
    using opengl_cpp::shader_type_t;

    auto ret = std::make_shared<opengl_cpp::program_t>(m_gl);
    ret->add_shader(shader_t(m_gl, shader_type_t::vertex, path("shaders/light.vert")));
    ret->add_shader(shader_t(m_gl, shader_type_t::fragment, path("shaders/light.frag")));
    ret->link();
    return ret;
}

shape_pointer_t integration_t::build_cube(texture_pointer_t &base_texture) {
    constexpr auto rotation_angle = 45.0F;
    constexpr auto scale = 0.5F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/cube.obj"));

    transform_t t;
    t.m_translation = {0.0F, 0.0F, -1.0F};
    t.m_rotation_angle = rotation_angle;
    t.m_rotation_axis = {0.0F, 1.0F, 0.0F};
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(default_ambient);
    mat.m_shininess = default_shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = build_texture("./textures/blue.png", texture_layer_2);
    mat.m_diffuse = build_texture("./textures/diffuse.png", texture_diffuse);
    mat.m_specular = build_texture("./textures/specular.png", texture_specular);

    ret->set_material(std::move(mat));
    return ret;
}

shape_pointer_t integration_t::build_plane(texture_pointer_t &base_texture) {
    constexpr auto scale = 10.0F;
    constexpr glm::vec3 position = {0.0F, -0.5F, 0.0F};

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/plane.obj"));

    transform_t t;
    t.m_translation = position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(default_ambient);
    mat.m_shininess = default_shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = build_texture("./textures/orange.png", texture_layer_2);
    mat.m_specular = build_texture("./textures/specular.png", texture_specular);
    ret->set_material(std::move(mat));
    return ret;
}

shape_pointer_t integration_t::build_sphere(texture_pointer_t &base_texture) {
    constexpr auto scale = 0.6F;
    constexpr auto ambient = 0.1F;
    constexpr auto shininess = 32.0F;
    constexpr glm::vec3 position = {1.5F, 0.0F, -1.0F};

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t;
    t.m_translation = position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(ambient);
    mat.m_shininess = shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = build_texture("./textures/red.png", texture_layer_2);
    mat.m_diffuse = build_texture("./textures/diffuse.png", texture_diffuse);
    mat.m_specular = build_texture("./textures/specular.png", texture_specular);

    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t integration_t::build_torus(texture_pointer_t &base_texture) {
    constexpr auto scale = 0.6F;
    constexpr auto shininess = 2.0F;
    constexpr auto rotation_angle = 45.0F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/torus.obj"));

    transform_t t;
    t.m_translation = {-1.0F, 0.0F, -1.0F};
    t.m_rotation_axis = {0.0F, 0.0F, 1.0F};
    t.m_rotation_angle = rotation_angle;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = {1.0F, 1.0F, 1.0F};
    mat.m_shininess = shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = build_texture("./textures/green.png", texture_layer_2);

    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t integration_t::build_light_shape(const light_pointer_t &light, texture_pointer_t &base_texture) {
    constexpr auto scale = 0.1F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t;
    t.m_translation = light->m_position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_texture1 = base_texture;
    ret->set_material(std::move(mat));

    m_light_shapes[light] = ret;
    return ret;
}

light_pointer_t integration_t::build_light(light_type_t type, glm::vec3 position, glm::vec3 direction) {
    constexpr auto ambient = 0.2F;
    constexpr auto diffuse = 1.0F;
    constexpr auto specular = 0.2F;
    constexpr auto attenuation_constant = 1.0F;
    constexpr auto attenuation_linear = 0.09F;
    constexpr auto attenuation_quadratic = 0.032F;

    light_pointer_t ret = std::make_unique<light_t>();
    switch (type) {
    case light_type_t::deactivated:
        return nullptr;
    case light_type_t::ambient:
        ret = std::make_unique<light_t>();
        break;
    case light_type_t::directional:
        ret = std::make_unique<directional_light_t>();
        break;
    case light_type_t::spot:
        ret = std::make_unique<spot_light_t>();
        break;
    }

    ret->m_position = std::move(position);
    ret->m_ambient = glm::vec3(ambient);
    ret->m_diffuse = glm::vec3(diffuse);
    ret->m_specular = glm::vec3(specular);
    ret->m_attenuation_constant = attenuation_constant;
    ret->m_attenuation_linear = attenuation_linear;
    ret->m_attenuation_quadratic = attenuation_quadratic;

    if (light_type_t::directional == type) {
        auto &direction_light = dynamic_cast<directional_light_t &>(*ret);
        direction_light.m_direction = std::move(direction);
        ret->m_attenuation_constant = 1.0F;
        ret->m_attenuation_linear = 0.0F;
        ret->m_attenuation_quadratic = 0.0F;
    } else if (light_type_t::spot == type) {
        constexpr auto cutoff_begin = 25.0F;
        constexpr auto cutoff_end = 35.0F;

        auto &spot_light = dynamic_cast<spot_light_t &>(*ret);
        spot_light.m_direction = std::move(direction);
        spot_light.m_cutoff_begin = cutoff_begin;
        spot_light.m_cutoff_end = cutoff_end;
    }

    return ret;
}

} // namespace game_engine