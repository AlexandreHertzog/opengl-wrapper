#pragma once

#include "exception.h"

namespace opengl_wrapper {

class glad_error_t : public exception_t {
  public:
    explicit glad_error_t(const std::string &description);
};

} // namespace opengl_wrapper
