#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

namespace opengl_wrapper {

class exception : public std::runtime_error {
  public:
    exception(std::string description);
};

} // namespace opengl_wrapper

#endif
