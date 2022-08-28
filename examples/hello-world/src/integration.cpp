#include "integration.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <boost/log/trivial.hpp>
#include <csignal>

namespace test_app {

integration::integration()
    : m_window(1920, 1080, "Test application"), m_camera({0.0, 3.0, 8.0}, {0.0, -0.3, -1.0}, {0.0, 1.0, 0.0}),
      m_default_callback([&](opengl_wrapper::program &p, opengl_wrapper::shape &s) {
          auto model = glm::mat4(1.0F);
          auto &transform = s.get_transform();
          model = glm::translate(model, transform.translation());
          model = glm::rotate(model, glm::radians(transform.rotation_angle()), transform.rotation_axis());
          model = glm::scale(model, transform.scale());

          auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
          auto projection = glm::perspective(glm::radians(45.0F), 1920.0F / 1080.0F, 0.1F, 100.0F);

          p.set_uniform("uniform_model", glm::value_ptr(model));
          p.set_uniform("uniform_view", glm::value_ptr(view));
          p.set_uniform("uniform_projection", glm::value_ptr(projection));
          if (!m_lights.empty()) {
              p.set_uniform("uniform_light_pos", m_lights[0].get_transform().translation());
          }
          p.set_uniform("uniform_specular", s.get_material().specular());
          p.set_uniform("uniform_shininess", s.get_material().shininess());
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
        w.draw(m_shapes);
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
    auto base_texture = opengl_wrapper::texture::build("./textures/checker.png", GL_TEXTURE0);

    m_shapes.emplace_back(build_cube(object_program, base_texture));
    m_shapes.emplace_back(build_plane(object_program, base_texture));
    m_shapes.emplace_back(build_sphere(object_program, base_texture));
    m_shapes.emplace_back(build_torus(object_program, base_texture));

    m_lights.emplace_back(build_light(light_program));

    for (auto &s : m_shapes) {
        s.set_uniform("uniform_texture1", 0);
        s.set_uniform("uniform_texture2", 1);
        s.set_uniform("uniform_texture_mix", 0.8F);
        s.set_uniform("uniform_light_color", glm::vec3(1.0F, 1.0F, 1.0F));
    }
}

void integration::prepare_render_loop() {
    m_window.set_depth_test(true);
    m_window.set_clear_color(0.2F, 0.2F, 0.2F, 1.0F);

    for (auto &shape : m_shapes) {
        shape.load_vertices();
    }

    for (auto &light : m_lights) {
        light.load_vertices();
    }
}

void integration::render_loop() {
    constexpr auto s_to_us_multiplier = 1000000.0;
    auto frame_time_us = s_to_us_multiplier / 60;

    while (!m_window.get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        build_ui();

        m_window.clear();

        m_window.draw(m_shapes);
        m_window.draw(m_lights);

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
    ImGui::Checkbox("Auto-rotate light", &m_auto_rotate_light);
    if (m_auto_rotate_light) {
        ImGui::BeginDisabled(true);
    }
    ImGui::InputFloat3("Light position", m_light_position);
    if (m_auto_rotate_light) {
        ImGui::EndDisabled();
    }
    ImGui::End();

    ImGui::Render();
}

std::shared_ptr<opengl_wrapper::program> integration::build_object_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/object.vert")));
    ret->add_shader(opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/object.frag")));
    ret->link();

    ret->set_use_callback(m_default_callback);
    return ret;
}

std::shared_ptr<opengl_wrapper::program> integration::build_light_program() {
    auto ret = std::make_shared<opengl_wrapper::program>();
    ret->add_shader(opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/light.vert")));
    ret->add_shader(opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/light.frag")));
    ret->link();

    ret->set_use_callback([&](opengl_wrapper::program &p, opengl_wrapper::shape &s) {
        constexpr auto radius = 4.0F;
        auto &transform = s.get_transform();
        if (m_auto_rotate_light) {
            transform.translate(radius * sin(glfwGetTime()), transform.translation().y, radius * cos(glfwGetTime()));
        } else {
            transform.translate(m_light_position[0], m_light_position[1], m_light_position[2]);
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
    ret.set_transform(opengl_wrapper::transform()
                          .translate(0.0F, 0.0F, -1.0F)
                          .rotate(45.0F, 0.0F, 1.0F, 0.0F)
                          .scale(0.5F, 0.5F, 0.5F));

    ret.set_program(object_program);

    ret.add_texture(base_texture);
    ret.add_texture(opengl_wrapper::texture::build("./textures/blue.png", GL_TEXTURE1));
    return ret;
}

opengl_wrapper::shape integration::build_plane(std::shared_ptr<opengl_wrapper::program> &object_program,
                                               opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/plane.obj"));
    ret.set_transform(opengl_wrapper::transform().translate(0.0F, -0.5F, 0.0F).scale(10.0F, 10.0F, 10.0F));
    ret.set_program(object_program);
    ret.add_texture(base_texture);
    ret.add_texture(opengl_wrapper::texture::build("./textures/orange.png", GL_TEXTURE1));
    return ret;
}

opengl_wrapper::shape integration::build_sphere(std::shared_ptr<opengl_wrapper::program> &object_program,
                                                opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));
    ret.set_transform(opengl_wrapper::transform().translate(1.5F, 0.0F, -1.0F).scale(0.6F, 0.6F, 0.6F));
    ret.set_program(object_program);

    ret.add_texture(base_texture);
    ret.add_texture(opengl_wrapper::texture::build("./textures/red.png", GL_TEXTURE1));

    ret.get_material().shininess(64.0F).specular(0.8F);

    return ret;
}

opengl_wrapper::shape integration::build_torus(std::shared_ptr<opengl_wrapper::program> &object_program,
                                               opengl_wrapper::texture::pointer_t &base_texture) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/torus.obj"));
    ret.set_transform(opengl_wrapper::transform()
                          .translate(-1.0F, 0.0F, -1.0F)
                          .rotate(45.0F, 0.0F, 0.0F, 1.0F)
                          .scale(0.6F, 0.6F, 0.6F));

    ret.set_program(object_program);

    ret.add_texture(base_texture);
    ret.add_texture(opengl_wrapper::texture::build("./textures/green.png", GL_TEXTURE1));

    ret.get_material().specular(128.0F).specular(0.3F);

    return ret;
}

opengl_wrapper::shape integration::build_light(std::shared_ptr<opengl_wrapper::program> &light_program) {
    opengl_wrapper::shape ret;
    ret.set_mesh(opengl_wrapper::mesh("./objects/sphere.obj"));
    ret.set_transform(opengl_wrapper::transform()
                          .translate(m_light_position[0], m_light_position[1], m_light_position[2])
                          .scale(0.1F, 0.1F, 0.1F));
    ret.set_program(light_program);
    ret.add_texture(opengl_wrapper::texture::build("./textures/white.png", GL_TEXTURE0));
    return ret;
}

} // namespace test_app