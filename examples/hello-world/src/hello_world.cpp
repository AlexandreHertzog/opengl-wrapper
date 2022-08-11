#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp" // No need to validate allocation problems here, it is a test program.

#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window.h"

#include <iostream>

const char *const vshader_source = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                   "}\0";

const char *const first_fshader_source = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                         "}\0";

const char *const second_fshader_source = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "void main()\n"
                                          "{\n"
                                          "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                          "}\0";

const std::vector<float> first_vertices = {-0.9F, -0.5F, 0.0F, -0.0F, -0.5F, 0.0F, -0.45F, 0.5F, 0.0F};

const std::vector<float> second_vertices = {0.0F, -0.5F, 0.0F, 0.9F, -0.5F, 0.0F, 0.45F, 0.5F, 0.0F};

const std::vector<unsigned int> indices = {0, 1, 2};

int main() {
    try {
        auto &gl_manager = opengl_wrapper::GlManager::instance();
        auto &window = opengl_wrapper::Window::instance();
        window.init(800, 600, "Hello world!"); // NOLINT(*-magic-numbers)

        window.setKeyAction(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.setWindowShouldClose(1);
            }
        });

        window.getRenderer().addShader(opengl_wrapper::Shader(GL_VERTEX_SHADER, vshader_source));
        window.getRenderer().addShader(opengl_wrapper::Shader(GL_FRAGMENT_SHADER, first_fshader_source));

        const auto first_program_id = window.getRenderer().linkProgram();

        window.getRenderer().addShader(opengl_wrapper::Shader(GL_VERTEX_SHADER, vshader_source));
        window.getRenderer().addShader(opengl_wrapper::Shader(GL_FRAGMENT_SHADER, second_fshader_source));

        const auto second_program_id = window.getRenderer().linkProgram();

        window.getRenderer().addVertices(first_vertices, indices, first_program_id);
        window.getRenderer().addVertices(second_vertices, indices, second_program_id);
        window.getRenderer().loadVertices();

        window.getRenderer().draw();

        window.renderLoop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop