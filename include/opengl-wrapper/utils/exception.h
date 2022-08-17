#ifndef OPENGL_WRAPPER_EXCEPTIONS_EXCEPTION_H
#define OPENGL_WRAPPER_EXCEPTIONS_EXCEPTION_H

#include <stdexcept>

namespace opengl_wrapper {

class exception : public std::runtime_error {
  public:
    explicit exception(const std::string &description);
};

} // namespace opengl_wrapper

#endif
