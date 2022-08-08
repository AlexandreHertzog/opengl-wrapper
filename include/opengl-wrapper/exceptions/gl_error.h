#ifndef OPENGL_WRAPPER_GL_ERROR_H
#define OPENGL_WRAPPER_GL_ERROR_H

#include "exception.h"

namespace opengl_wrapper {
class GlError : public Exception {
  public:
    GlError(std::string message);
};
} // namespace opengl_wrapper

#endif