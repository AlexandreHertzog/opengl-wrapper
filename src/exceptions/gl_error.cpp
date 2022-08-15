#include "gl_error.h"

namespace opengl_wrapper {

gl_error::gl_error(const std::string &description) : exception(description) {
}

} // namespace opengl_wrapper
