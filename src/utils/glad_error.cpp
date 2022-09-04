#include "opengl-wrapper/utils/glad_error.h"

namespace opengl_wrapper {

glad_error_t::glad_error_t(const std::string &description) : exception_t(description) {
}

} // namespace opengl_wrapper
