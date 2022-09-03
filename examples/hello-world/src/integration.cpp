#include "integration.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <boost/log/trivial.hpp>
#include <csignal>

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

} // namespace

namespace test_app {

integration::integration()
    : m_window(resolution_x, resolution_y, "Test application"),
      m_camera(default_camera_pos, default_camera_front, default_camera_up) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

integration::~integration() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void integration::init_callbacks() {
    constexpr auto camera_speed = 0.1F;

    m_window.set_framebuffer_callback([&](opengl_wrapper::window &w, int width, int height) {
        w.set_viewport(width, height);
        render();
    });

    m_window.set_key_callback([&](opengl_wrapper::window &w, int key, int scancode, int action, int mods) {
        if (GLFW_PRESS == action && GLFW_KEY_ESCAPE == key) {
            w.set_should_close(1);
        }
        if (GLFW_PRESS == action && GLFW_KEY_F12 == key) {
            m_wireframe = !m_wireframe;
            w.set_wireframe_mode(m_wireframe);
        }
        if (GLFW_PRESS == action && GLFW_KEY_SPACE == key) {
            m_cursor_enabled = !m_cursor_enabled;
            w.set_input_mode(GLFW_CURSOR, m_cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
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

    m_window.set_cursor_pos_callback([&](opengl_wrapper::window &w, double xpos, double ypos) {
        if (m_first_cursor_iteration) {
            m_last_cursor_xpos = xpos;
            m_last_cursor_ypos = ypos;
            m_first_cursor_iteration = false;
        }

        constexpr auto sensitivity = 0.1;
        const auto xpos_offset = (xpos - m_last_cursor_xpos) * sensitivity;
        const auto ypos_offset = (m_last_cursor_ypos - ypos) * sensitivity;

        m_last_cursor_xpos = xpos;
        m_last_cursor_ypos = ypos;

        m_yaw += xpos_offset;
        m_pitch = std::max(std::min(m_pitch + ypos_offset, camera_max_angle), camera_min_angle);

        if (!m_cursor_enabled) {
            m_camera.set_front(m_pitch, m_yaw);
        }
    });
}

void integration::build_shapes() {
    auto object_program = build_object_program();
    auto light_program = build_light_program();
    auto base_texture = opengl_wrapper::texture::build("./textures/checker.png", texture_layer_1);

    m_program_shape_map[object_program].emplace_back(build_cube(base_texture));
    m_program_shape_map[object_program].emplace_back(build_plane(base_texture));
    m_program_shape_map[object_program].emplace_back(build_sphere(base_texture));
    m_program_shape_map[object_program].emplace_back(build_torus(base_texture));

    m_light = build_light(light_type_t::spot);
    m_program_shape_map[light_program].emplace_back(m_light->m_shape);
}

void integration::render_loop() {
    constexpr opengl_wrapper::color_alpha_t clear_color = {0.2F, 0.2F, 0.2F, 1.0F};

    m_window.set_depth_test(true);
    m_window.set_clear_color(clear_color);

    for (auto &program_shapes : m_program_shape_map) {
        for (auto &shape : program_shapes.second) {
            shape.load_vertices();
        }
    }

    assert(m_light);
    m_light->m_shape.load_vertices();

    while (!m_window.get_should_close()) {
        render();
    }
}
void integration::render() {
    constexpr auto s_to_us_multiplier = 1000000.0;
    auto frame_time_us = s_to_us_multiplier / refresh_rate;

    auto start_time = std::chrono::high_resolution_clock::now();

    build_ui();

    m_window.clear();

    for (auto &program_shape : m_program_shape_map) {
        assert(program_shape.first);
        program_shape.first->use();

        update_projection_uniforms(*program_shape.first);
        update_light_uniforms(*program_shape.first);

        for (auto &shape : program_shape.second) {
            update_shape_uniforms(*program_shape.first, shape);
            m_window.draw(shape);
        }
    }

    assert(m_light);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.poll_events();

    std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

    if (loop_time_us.count() < frame_time_us) {
        const auto wait_time_us = frame_time_us - loop_time_us.count();
        BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
        usleep(static_cast<unsigned int>(wait_time_us));
    } else {
        BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
    }

    m_window.swap_buffers();
}

void integration::build_ui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //    ImGui::ShowDemoWindow();

    ImGui::Begin("OpenGL Wrapper test app");

    if (ImGui::CollapsingHeader("Lights")) {
        assert(m_light);

        ImGui::InputScalarN("Light position", ImGuiDataType_Float, &m_light->m_shape.get_transform().m_translation, 3);

        auto *directional_light = dynamic_cast<opengl_wrapper::directional_light *>(m_light.get());
        if (nullptr != directional_light) {
            ImGui::InputScalarN("Direction", ImGuiDataType_Float, &directional_light->m_direction, 3);
        } else {
            auto *spot_light = dynamic_cast<opengl_wrapper::spot_light *>(m_light.get());
            if (nullptr != spot_light) {
                constexpr auto min_cutoff_begin = 5.0F;
                constexpr auto max_cutoff_end = 120.0F;

                ImGui::InputScalarN("Direction", ImGuiDataType_Float, &spot_light->m_direction, 3);
                ImGui::SliderFloat("Cutoff begin", &spot_light->m_cutoff_begin, min_cutoff_begin,
                                   spot_light->m_cutoff_end);
                ImGui::SliderFloat("Cutoff end", &spot_light->m_cutoff_end, spot_light->m_cutoff_begin, max_cutoff_end);
            }
        }

        const float min_material = 0.0F;
        const float max_material = 2.0F;
        ImGui::SliderScalarN("Ambient", ImGuiDataType_Float, &m_light->m_ambient, 3, &min_material, &max_material);
        ImGui::SliderScalarN("Diffuse", ImGuiDataType_Float, &m_light->m_diffuse, 3, &min_material, &max_material);
        ImGui::SliderScalarN("Specular", ImGuiDataType_Float, &m_light->m_specular, 3, &min_material, &max_material);
        ImGui::InputFloat("Constant attenuation", &m_light->m_attenuation_constant);
        ImGui::InputFloat("Linear attenuation", &m_light->m_attenuation_linear);
        ImGui::InputFloat("Linear quadratic", &m_light->m_attenuation_quadratic);
    }

    for (auto &program_shapes : m_program_shape_map) {
        for (auto &shape : program_shapes.second) {
            if (ImGui::CollapsingHeader(shape.get_mesh().get_name().c_str())) {
                shape_debug_ui(shape);
            }
        }
    }

    ImGui::End();

    ImGui::Render();
}

void integration::update_light_uniforms(opengl_wrapper::program &p) {
    if (m_light) {
        p.set_uniform("uniform_light.position", m_light->m_shape.get_transform().m_translation);
        p.set_uniform("uniform_light.ambient", m_light->m_ambient);
        p.set_uniform("uniform_light.diffuse", m_light->m_diffuse);
        p.set_uniform("uniform_light.specular", m_light->m_specular);
        p.set_uniform("uniform_light.attenuation_constant", m_light->m_attenuation_constant);
        p.set_uniform("uniform_light.attenuation_linear", m_light->m_attenuation_linear);
        p.set_uniform("uniform_light.attenuation_quadratic", m_light->m_attenuation_quadratic);

        auto *direction_light = dynamic_cast<opengl_wrapper::directional_light *>(m_light.get());
        if (nullptr != direction_light) {
            p.set_uniform("uniform_light.type", static_cast<int>(light_type_t::directional));
            p.set_uniform("uniform_light.direction", direction_light->m_direction);
        } else {
            auto *spot_light = dynamic_cast<opengl_wrapper::spot_light *>(m_light.get());
            if (nullptr != spot_light) {
                p.set_uniform("uniform_light.type", static_cast<int>(light_type_t::spot));
                p.set_uniform("uniform_light.direction", spot_light->m_direction);
                p.set_uniform("uniform_light.cutoff_begin", glm::cos(glm::radians(spot_light->m_cutoff_begin)));
                p.set_uniform("uniform_light.cutoff_end", glm::cos(glm::radians(spot_light->m_cutoff_end)));
            }
        }
    }
}

void integration::update_projection_uniforms(opengl_wrapper::program &p) {
    auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
    p.set_uniform("uniform_view", view);

    auto projection = glm::perspective(glm::radians(default_fov), resolution_ratio, clipping_near, clipping_far);
    p.set_uniform("uniform_projection", projection);
}

void integration::update_shape_uniforms(opengl_wrapper::program &p, opengl_wrapper::shape &s) {
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

void integration::shape_debug_ui(opengl_wrapper::shape &s) {
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

std::shared_ptr<opengl_wrapper::program> integration::build_object_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(opengl_wrapper::shader(opengl_wrapper::shader_type_t::vertex, path("shaders/object.vert")));
    ret->add_shader(opengl_wrapper::shader(opengl_wrapper::shader_type_t::fragment, path("shaders/object.frag")));
    ret->link();
    return ret;
}

std::shared_ptr<opengl_wrapper::program> integration::build_light_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(opengl_wrapper::shader(opengl_wrapper::shader_type_t::vertex, path("shaders/light.vert")));
    ret->add_shader(opengl_wrapper::shader(opengl_wrapper::shader_type_t::fragment, path("shaders/light.frag")));
    ret->link();
    return ret;
}

opengl_wrapper::shape integration::build_cube(opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/cube.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {0.0F, 0.0F, -1.0F};
    t.m_rotation_angle = 45.0F;
    t.m_rotation_axis = {0.0F, 1.0F, 0.0F};
    t.m_scale = {0.5F, 0.5F, 0.5F};
    ret.set_transform(t);

    opengl_wrapper::material mat;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/blue.png", texture_layer_2);
    mat.m_diffuse = opengl_wrapper::texture::build("./textures/diffuse.png", texture_diffuse);
    mat.m_specular = opengl_wrapper::texture::build("./textures/specular.png", texture_specular);

    ret.set_material(std::move(mat));
    return ret;
}

opengl_wrapper::shape integration::build_plane(opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/plane.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {0.0F, -0.5F, 0.0F};
    t.m_scale = {10.0F, 10.0F, 10.0F};
    ret.set_transform(t);

    opengl_wrapper::material mat;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/orange.png", texture_layer_2);
    mat.m_specular = opengl_wrapper::texture::build("./textures/specular.png", texture_specular);
    ret.set_material(std::move(mat));
    return ret;
}

opengl_wrapper::shape integration::build_sphere(opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {1.5F, 0.0F, -1.0F};
    t.m_scale = {0.6F, 0.6F, 0.6F};
    ret.set_transform(t);

    opengl_wrapper::material mat;
    mat.m_ambient = {0.1F, 0.1F, 0.1F};
    mat.m_shininess = 32.0F;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/red.png", texture_layer_2);
    mat.m_diffuse = opengl_wrapper::texture::build("./textures/diffuse.png", texture_diffuse);
    mat.m_specular = opengl_wrapper::texture::build("./textures/specular.png", texture_specular);

    ret.set_material(std::move(mat));

    return ret;
}

opengl_wrapper::shape integration::build_torus(opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/torus.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {-1.0F, 0.0F, -1.0F};
    t.m_rotation_axis = {0.0F, 0.0F, 1.0F};
    t.m_rotation_angle = 45.0F;
    t.m_scale = {0.6F, 0.6F, 0.6F};
    ret.set_transform(t);

    opengl_wrapper::material mat;
    mat.m_ambient = {1.0F, 1.0F, 1.0F};
    mat.m_shininess = 2.0F;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/green.png", texture_layer_2);

    ret.set_material(std::move(mat));

    return ret;
}

std::unique_ptr<opengl_wrapper::light> integration::build_light(light_type_t type) {
    constexpr auto ambient = 0.2F;
    constexpr auto diffuse = 1.0F;
    constexpr auto specular = 0.2F;
    constexpr auto scale = 0.1F;
    constexpr auto position = 2.0F;
    constexpr auto direction_x = -1.0F;
    constexpr auto direction_y = -2.0F;
    constexpr auto direction_z = -2.0F;
    constexpr auto attenuation_constant = 1.0F;
    constexpr auto attenuation_linear = 0.09F;
    constexpr auto attenuation_quadratic = 0.032F;

    std::unique_ptr<opengl_wrapper::light> ret = std::make_unique<opengl_wrapper::light>();
    switch (type) {
    case light_type_t::ambient:
        ret = std::make_unique<opengl_wrapper::light>();
        break;
    case light_type_t::directional:
        ret = std::make_unique<opengl_wrapper::directional_light>();
        break;
    case light_type_t::spot:
        ret = std::make_unique<opengl_wrapper::spot_light>();
        break;
    }

    ret->m_ambient = glm::vec3(ambient);
    ret->m_diffuse = glm::vec3(diffuse);
    ret->m_specular = glm::vec3(specular);
    ret->m_attenuation_constant = attenuation_constant;
    ret->m_attenuation_linear = attenuation_linear;
    ret->m_attenuation_quadratic = attenuation_quadratic;

    ret->m_shape.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));

    opengl_wrapper::transform t;
    t.m_translation = glm::vec3(position);
    t.m_scale = glm::vec3(scale);
    ret->m_shape.set_transform(t);

    opengl_wrapper::material mat;
    mat.m_texture1 = opengl_wrapper::texture::build("./textures/white.png", texture_layer_1);

    ret->m_shape.set_material(std::move(mat));

    if (light_type_t::directional == type) {
        auto &direction_light = dynamic_cast<opengl_wrapper::directional_light &>(*ret);
        direction_light.m_direction = glm::vec3(direction_x, direction_y, direction_z);
        ret->m_attenuation_constant = 1.0F;
        ret->m_attenuation_linear = 0.0F;
        ret->m_attenuation_quadratic = 0.0F;
    } else if (light_type_t::spot == type) {
        constexpr auto cutoff_begin = 25.0F;
        constexpr auto cutoff_end = 35.0F;

        auto &spot_light = dynamic_cast<opengl_wrapper::spot_light &>(*ret);
        spot_light.m_direction = glm::vec3(direction_x, direction_y, direction_z);
        spot_light.m_cutoff_begin = cutoff_begin;
        spot_light.m_cutoff_end = cutoff_end;
    }

    return ret;
}

} // namespace test_app