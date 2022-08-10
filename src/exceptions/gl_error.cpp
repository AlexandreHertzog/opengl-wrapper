#include "gl_error.h"

namespace opengl_wrapper {

GlError::GlError(const std::string &description) : Exception(description) {
}

} // namespace opengl_wrapper
