#ifndef FORMATTER_H
#define FORMATTER_H

#include <GLFW/glfw3.h>
#include <string>

namespace opengl_wrapper {
class Formatter {
    Formatter() = delete;
    ~Formatter() = delete;

  public:
    static std::string format_glfw_error_code(int error_code);
};
} // namespace opengl_wrapper

#endif