#include "integration.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <boost/log/trivial.hpp>
#include <csignal>

namespace {
constexpr auto texture_layer_1 = 0;
constexpr auto texture_layer_2 = 1;
constexpr auto texture_diffuse = 2;
constexpr auto texture_specular = 3;
} // namespace

namespace test_app {

integration::integration()
    : m_window(1920, 1080, "Test application"), m_camera({0.0, 3.0, 8.0}, {0.0, -0.3, -1.0}, {0.0, 1.0, 0.0}),
      m_default_callback([&](opengl_wrapper::program &p, opengl_wrapper::shape &s) {
          auto model = glm::mat4(1.0F);
          auto &transform = s.get_transform();
          model = glm::translate(model, transform.m_translation);
          model = glm::rotate(model, glm::radians(transform.m_rotation_angle), transform.m_rotation_axis);
          model = glm::scale(model, transform.m_scale);

          auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
          auto projection = glm::perspective(glm::radians(45.0F), 1920.0F / 1080.0F, 0.1F, 100.0F);

          p.set_uniform("uniform_model", model);
          p.set_uniform("uniform_view", view);
          p.set_uniform("uniform_projection", projection);

          if (!m_lights.empty()) {
              p.set_uniform("uniform_light.position", m_lights[0].m_shape.get_transform().m_translation);
              p.set_uniform("uniform_light.ambient", m_lights[0].m_ambient);
              p.set_uniform("uniform_light.diffuse", m_lights[0].m_diffuse);
              p.set_uniform("uniform_light.specular", m_lights[0].m_specular);
          }

          p.set_uniform("uniform_material.has_diffuse", static_cast<bool>(s.get_material().m_diffuse));
          p.set_uniform("uniform_material.ambient", s.get_material().m_ambient);
          p.set_uniform("uniform_material.has_specular", static_cast<bool>(s.get_material().m_specular));
          p.set_uniform("uniform_material.shininess", s.get_material().m_shininess);
          p.set_uniform("uniform_material.texture1", 0);
          p.set_uniform("uniform_material.texture2", 1);
          p.set_uniform("uniform_material.diffuse", 2);
          p.set_uniform("uniform_material.specular", 3);
          p.set_uniform("uniform_material.texture_mix", s.get_material().m_texture_mix);
      }) {

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
        for (auto &s : m_shapes) {
            w.draw(s);
        }
    });

    m_window.set_key_callback([&](opengl_wrapper::window &w, int key, int scancode, int action, int mods) {
        if (GLFW_PRESS == action && GLFW_KEY_ESCAPE == key) {
            w.set_should_close(1);
        }
        if (GLFW_PRESS == action && GLFW_KEY_BACKSPACE == key) {
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
        if (m_cursor_enabled) {
            return;
        }

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
        m_pitch = std::max(std::min(m_pitch + ypos_offset, 89.0), -89.0);

        m_camera.set_front(m_pitch, m_yaw);
    });
}

void integration::build_shapes() {
    auto object_program = build_object_program();
    auto light_program = build_light_program();
    auto base_texture = opengl_wrapper::texture::build("./textures/checker.png", texture_layer_1);

    m_shapes.emplace_back(build_cube(object_program, base_texture));
    m_shapes.emplace_back(build_plane(object_program, base_texture));
    m_shapes.emplace_back(build_sphere(object_program, base_texture));
    m_shapes.emplace_back(build_torus(object_program, base_texture));

    m_lights.emplace_back(build_light(light_program));
}

void integration::prepare_render_loop() {
    constexpr opengl_wrapper::color_alpha_t clear_color = {0.2F, 0.2F, 0.2F, 1.0F};

    m_window.set_depth_test(true);
    m_window.set_clear_color(clear_color);

    for (auto &shape : m_shapes) {
        shape.load_vertices();
    }

    for (auto &light : m_lights) {
        light.m_shape.load_vertices();
    }
}

void integration::render_loop() {
    constexpr auto s_to_us_multiplier = 1000000.0;
    auto frame_time_us = s_to_us_multiplier / 60;

    while (!m_window.get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        build_ui();

        m_window.clear();

        for (auto &s : m_shapes) {
            m_window.draw(s);
        }

        for (auto &l : m_lights) {
            m_window.draw(l.m_shape);
        }

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
}

void integration::build_ui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //    ImGui::ShowDemoWindow();

    ImGui::Begin("OpenGL Wrapper test app");

    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::Checkbox("Auto-rotate light", &m_auto_rotate_light);
        if (m_auto_rotate_light) {
            ImGui::BeginDisabled(true);
        }
        ImGui::InputScalarN("Light position", ImGuiDataType_Float,
                            &m_lights.front().m_shape.get_transform().m_translation, 3);
        if (m_auto_rotate_light) {
            ImGui::EndDisabled();
        }

        float min_material = 0.0F;
        float max_material = 2.0F;
        ImGui::SliderScalarN("Ambient", ImGuiDataType_Float, &m_lights.front().m_ambient, 3, &min_material,
                             &max_material);
        ImGui::SliderScalarN("Diffuse", ImGuiDataType_Float, &m_lights.front().m_diffuse, 3, &min_material,
                             &max_material);
        ImGui::SliderScalarN("Specular", ImGuiDataType_Float, &m_lights.front().m_specular, 3, &min_material,
                             &max_material);
    }

    if (ImGui::CollapsingHeader("Cube")) {
        shape_debug_ui(m_shapes[0]);
    }
    if (ImGui::CollapsingHeader("Plane")) {
        shape_debug_ui(m_shapes[1]);
    }
    if (ImGui::CollapsingHeader("Sphere")) {
        shape_debug_ui(m_shapes[2]);
    }
    if (ImGui::CollapsingHeader("Torus")) {
        shape_debug_ui(m_shapes[3]);
    }

    ImGui::End();

    ImGui::Render();
}

void integration::shape_debug_ui(opengl_wrapper::shape &s) {
    float min_translation = -10.0F;
    float max_translation = 10.0F;

    ImGui::SliderScalarN("Position", ImGuiDataType_Float, &s.get_transform().m_translation, 3, &min_translation,
                         &max_translation);
    ImGui::SliderFloat("Rot angle", &s.get_transform().m_rotation_angle, -180.0F, 180.0F);
    ImGui::InputScalarN("Rot axis", ImGuiDataType_Float, &s.get_transform().m_rotation_axis, 3);
    ImGui::InputScalarN("Scale", ImGuiDataType_Float, &s.get_transform().m_scale, 3);

    float min_material = 0.0F;
    float max_material = 2.0F;
    ImGui::SliderScalarN("Ambient", ImGuiDataType_Float, &s.get_material().m_ambient, 3, &min_material, &max_material);
    ImGui::SliderScalarN("Diffuse", ImGuiDataType_Float, &s.get_material().m_diffuse, 3, &min_material, &max_material);
    ImGui::SliderFloat("Shininess", &s.get_material().m_shininess, 0.0F, 10.0F);
    ImGui::SliderScalarN("Specular", ImGuiDataType_Float, &s.get_material().m_specular, 3, &min_material,
                         &max_material);
}

std::shared_ptr<opengl_wrapper::program> integration::build_object_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(
        opengl_wrapper::shader(opengl_wrapper::shader_type_t::vertex, std::filesystem::path("shaders/object.vert")));
    ret->add_shader(
        opengl_wrapper::shader(opengl_wrapper::shader_type_t::fragment, std::filesystem::path("shaders/object.frag")));
    ret->link();

    ret->set_use_callback(m_default_callback);
    return ret;
}

std::shared_ptr<opengl_wrapper::program> integration::build_light_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(
        opengl_wrapper::shader(opengl_wrapper::shader_type_t::vertex, std::filesystem::path("shaders/light.vert")));
    ret->add_shader(
        opengl_wrapper::shader(opengl_wrapper::shader_type_t::fragment, std::filesystem::path("shaders/light.frag")));
    ret->link();

    ret->set_use_callback([&](opengl_wrapper::program &p, opengl_wrapper::shape &s) {
        constexpr auto radius = 4.0F;
        auto &transform = s.get_transform();
        if (m_auto_rotate_light) {
            transform.m_translation.x = radius * sin(glfwGetTime());
            transform.m_translation.z = radius * cos(glfwGetTime());
        }
        p.set_uniform("uniform_view_pos", m_camera.get_position());
        m_default_callback(p, s);
    });
    return ret;
}

opengl_wrapper::shape integration::build_cube(std::shared_ptr<opengl_wrapper::program> &object_program,
                                              opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/cube.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {0.0F, 0.0F, -1.0F};
    t.m_rotation_angle = 45.0F;
    t.m_rotation_axis = {0.0F, 1.0F, 0.0F};
    t.m_scale = {0.5F, 0.5F, 0.5F};

    ret.set_transform(t);

    ret.set_program(object_program);

    opengl_wrapper::material mat;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/blue.png", texture_layer_2);
    mat.m_diffuse = opengl_wrapper::texture::build("./textures/diffuse.png", texture_diffuse);
    mat.m_specular = opengl_wrapper::texture::build("./textures/specular.png", texture_specular);

    ret.set_material(std::move(mat));
    return ret;
}

opengl_wrapper::shape integration::build_plane(std::shared_ptr<opengl_wrapper::program> &object_program,
                                               opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/plane.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {0.0F, -0.5F, 0.0F};
    t.m_scale = {10.0F, 10.0F, 10.0F};

    ret.set_transform(t);
    ret.set_program(object_program);

    opengl_wrapper::material mat;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/orange.png", texture_layer_2);
    mat.m_specular = opengl_wrapper::texture::build("./textures/specular.png", texture_specular);
    ret.set_material(std::move(mat));
    return ret;
}

opengl_wrapper::shape integration::build_sphere(std::shared_ptr<opengl_wrapper::program> &object_program,
                                                opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {1.5F, 0.0F, -1.0F};
    t.m_scale = {0.6F, 0.6F, 0.6F};

    ret.set_transform(t);
    ret.set_program(object_program);

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

opengl_wrapper::shape integration::build_torus(std::shared_ptr<opengl_wrapper::program> &object_program,
                                               opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/torus.obj"));

    opengl_wrapper::transform t;
    t.m_translation = {-1.0F, 0.0F, -1.0F};
    t.m_rotation_axis = {0.0F, 0.0F, 1.0F};
    t.m_rotation_angle = 45.0F;
    t.m_scale = {0.6F, 0.6F, 0.6F};

    ret.set_transform(t);

    ret.set_program(object_program);

    opengl_wrapper::material mat;
    mat.m_ambient = {1.0F, 1.0F, 1.0F};
    mat.m_shininess = 2.0F;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = opengl_wrapper::texture::build("./textures/green.png", texture_layer_2);

    ret.set_material(std::move(mat));

    return ret;
}

opengl_wrapper::light integration::build_light(std::shared_ptr<opengl_wrapper::program> &light_program) {
    opengl_wrapper::light ret;
    ret.m_shape.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));

    opengl_wrapper::transform t;
    t.m_scale = {0.1F, 0.1F, 0.1F};

    ret.m_shape.set_transform(t);
    ret.m_shape.set_program(light_program);

    opengl_wrapper::material mat;
    mat.m_texture1 = opengl_wrapper::texture::build("./textures/white.png", texture_layer_1);

    ret.m_shape.set_material(std::move(mat));
    return ret;
}

} // namespace test_app