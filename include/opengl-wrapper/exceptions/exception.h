#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

namespace opengl_wrapper {

class Exception : public std::runtime_error {
  public:
    explicit Exception(const std::string &description);
};

} // namespace opengl_wrapper

#endif
