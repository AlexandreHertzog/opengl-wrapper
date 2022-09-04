#pragma once

#include "exception.h"

namespace opengl_wrapper {

class gl_error_t : public exception_t {
  public:
    explicit gl_error_t(const std::string &message);
};

} // namespace opengl_wrapper
