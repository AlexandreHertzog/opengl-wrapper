#pragma once

#include "exception.h"

namespace opengl_wrapper {

class glad_error : public exception {
  public:
    explicit glad_error(const std::string &description);
};

} // namespace opengl_wrapper
