#ifndef FORMATTER_H
#define FORMATTER_H

#include <GLFW/glfw3.h>
#include <string>

namespace opengl_wrapper {
class Formatter {
  public:
    Formatter() = delete;
    ~Formatter() = delete;
    static std::string formatGlfwErrorCode(int error_code);
};
} // namespace opengl_wrapper

#endif