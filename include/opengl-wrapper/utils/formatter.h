#ifndef OPENGL_WRAPPER_UTILS_FORMATTER_H
#define OPENGL_WRAPPER_UTILS_FORMATTER_H

#include <string>

namespace opengl_wrapper {
class formatter {
  public:
    static std::string format_glfw_error_code(int error_code);
};
} // namespace opengl_wrapper

#endif