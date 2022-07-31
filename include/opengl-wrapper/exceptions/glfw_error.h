#ifndef GLFW_ERROR_H
#define GLFW_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class glfw_error : public exception {
  public:
    glfw_error(std::string description);
};

} // namespace opengl_wrapper

#endif
