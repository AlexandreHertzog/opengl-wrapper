#ifndef OPENGL_WRAPPER_EXCEPTIONS_GLAD_ERROR_H
#define OPENGL_WRAPPER_EXCEPTIONS_GLAD_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class glad_error : public exception {
  public:
    explicit glad_error(const std::string &description);
};

} // namespace opengl_wrapper

#endif
