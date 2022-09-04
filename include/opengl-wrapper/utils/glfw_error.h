#pragma once

#include "exception.h"

namespace opengl_wrapper {

class glfw_error_t : public exception_t {
  public:
    explicit glfw_error_t(const std::string &description);
};

} // namespace opengl_wrapper
