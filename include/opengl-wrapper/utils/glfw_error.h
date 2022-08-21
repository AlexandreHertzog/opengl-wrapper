#pragma once

#include "exception.h"

namespace opengl_wrapper {

class glfw_error : public exception {
  public:
    explicit glfw_error(const std::string &description);
};

} // namespace opengl_wrapper
