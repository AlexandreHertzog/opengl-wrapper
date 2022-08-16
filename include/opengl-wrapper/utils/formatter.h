#ifndef OPENGL_WRAPPER_UTILS_FORMATTER_H
#define OPENGL_WRAPPER_UTILS_FORMATTER_H

#include <GLFW/glfw3.h>
#include <string>

namespace opengl_wrapper {
class Formatter {
  public:
    static std::string formatGlfwErrorCode(int error_code);
};
} // namespace opengl_wrapper

#endif