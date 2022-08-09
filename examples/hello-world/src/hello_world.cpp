#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window.h"

#include <iostream>

const char *vshader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fshader_source = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "void main()\n"
                             "{\n"
                             "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                             "}\0";

const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                          0.0f,  0.0f,  0.5f, 0.0f};

int main(int, char **) {
    try {
        auto &gl_manager = opengl_wrapper::GlManager::instance();
        auto &window = opengl_wrapper::Window::instance();
        window.init(800, 600, "Hello world!");

        window.setKeyAction(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.setWindowShouldClose(true);
            }
        });

        window.getRenderer().addVertices(sizeof(vertices), vertices);

        window.getRenderer().addShader(
            opengl_wrapper::Shader(GL_VERTEX_SHADER, vshader_source));
        window.getRenderer().addShader(
            opengl_wrapper::Shader(GL_FRAGMENT_SHADER, fshader_source));

        window.getRenderer().useProgram();
        window.getRenderer().draw(GL_TRIANGLES);

        window.renderLoop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}
