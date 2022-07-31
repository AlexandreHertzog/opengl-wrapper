#ifndef FORMATTER_H
#define FORMATTER_H

#include <GLFW/glfw3.h>
#include <string>

namespace opengl_wrapper {
class Formatter {
    Formatter() = delete;
    ~Formatter() = delete;

  public:
    static std::string formatGlfwErrorCode(int error_code);
    static std::string formatGlfwWindow(GLFWwindow *window);
};
} // namespace opengl_wrapper

#endif