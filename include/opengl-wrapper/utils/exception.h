#pragma once

#include <stdexcept>

namespace opengl_wrapper {

class exception_t : public std::runtime_error {
  public:
    explicit exception_t(const std::string &description);
};

} // namespace opengl_wrapper
