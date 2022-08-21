#pragma once

#include "exception.h"

namespace opengl_wrapper {

class gl_error : public exception {
  public:
    explicit gl_error(const std::string &message);
};

} // namespace opengl_wrapper
