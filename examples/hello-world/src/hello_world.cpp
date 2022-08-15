#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp" // No need to validate allocation problems here, it is a test program.

#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window_manager.h"
#include <boost/log/trivial.hpp>
#include <complex>
#include <iostream>

const std::vector<float> first_vertices = {-0.9F, -0.5F, 0.0F, 1.0F,   1.0F, 1.0F, -0.0F, -0.5F, 0.0F,
                                           1.0F,  1.0F,  1.0F, -0.45F, 0.5F, 0.0F, 1.0F,  1.0F,  1.0F};

const std::vector<float> second_vertices = {0.0F, -0.5F, 0.0F, 1.0F,  0.0F, 0.0F, 0.9F, -0.5F, 0.0F,
                                            0.0F, 0.1F,  0.0F, 0.45F, 0.5F, 0.0F, 0.0F, 0.0F,  1.0F};

const std::vector<unsigned int> indices = {0, 1, 2};

int main() {
    try {
        auto &gl_manager = opengl_wrapper::GlManager::instance();
        auto &window = opengl_wrapper::WindowManager::instance();
        window.init(800, 600, "Hello world!"); // NOLINT(*-magic-numbers)

        window.setKeyAction(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.setWindowShouldClose(1);
            }
        });

        auto first_program = std::make_shared<opengl_wrapper::Program>();

        first_program->addShader(
            opengl_wrapper::Shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/shader.vert")));

        first_program->addShader(
            opengl_wrapper::Shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/triangle1.frag")));

        first_program->link();

        first_program->setUseCallback([](opengl_wrapper::Program &program) {
            auto time_value = glfwGetTime();
            auto green_value = sin(time_value) / 2.0F + 0.5F; // NOLINT(*-magic-numbers)
            int runtime_color = program.getUniformLocation("runtime_color");
            glUniform4f(runtime_color, 0.0F, static_cast<float>(green_value), 0.0F, 1.0F);
        });

        auto second_program = std::make_shared<opengl_wrapper::Program>();

        second_program->addShader(
            opengl_wrapper::Shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/shader.vert")));

        second_program->addShader(
            opengl_wrapper::Shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/triangle2.frag")));

        second_program->link();

        window.getRenderer().addVertices(first_vertices, indices, first_program);
        window.getRenderer().addVertices(second_vertices, indices, second_program);
        window.getRenderer().loadVertices();

        window.renderLoop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop