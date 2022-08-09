#ifndef GLFW_ERROR_H
#define GLFW_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class GlfwError : public Exception {
  public:
    explicit GlfwError(const std::string &description);
};

} // namespace opengl_wrapper

#endif
