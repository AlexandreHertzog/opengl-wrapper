#ifndef OPENGL_WRAPPER_EXCEPTIONS_GLFW_ERROR_H
#define OPENGL_WRAPPER_EXCEPTIONS_GLFW_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class glfw_error : public exception {
  public:
    explicit glfw_error(const std::string &description);
};

} // namespace opengl_wrapper

#endif
