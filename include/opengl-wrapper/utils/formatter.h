#pragma once

#include <string>

namespace opengl_wrapper {

class formatter_t {
  public:
    static std::string format_glfw_error_code(int error_code);
};

} // namespace opengl_wrapper