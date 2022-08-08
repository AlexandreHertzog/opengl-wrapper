#include "gl_error.h"

namespace opengl_wrapper {
GlError::GlError(std::string description) : Exception(description) {}
} // namespace opengl_wrapper
