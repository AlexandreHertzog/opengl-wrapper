#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

namespace opengl_wrapper {

class Exception : public std::runtime_error {
  public:
    Exception(std::string description);
};

} // namespace opengl_wrapper

#endif
