#include "opengl-wrapper/utils/gl_error.h"

namespace opengl_wrapper {

gl_error_t::gl_error_t(const std::string &description) : exception_t(description) {
}

} // namespace opengl_wrapper
