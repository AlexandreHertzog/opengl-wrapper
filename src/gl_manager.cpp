#include "gl_manager.h"

#include <boost/log/trivial.hpp>

#include "opengl-wrapper/utils/glfw_error.h"
#include "utils/formatter.h"
#include "wraps/api.h"

namespace opengl_wrapper {

const GLFWerrorfun gl_manager::error_handler_ = [](int error_code, // NOLINT(cert-err58-cpp)
                                                   const char *description) {
    BOOST_LOG_TRIVIAL(error) << "GLFW error: error_code=" << Formatter::formatGlfwErrorCode(error_code)
                             << ", description=\"" << description << "\"";
};

gl_manager::gl_manager() {
    BOOST_LOG_TRIVIAL(debug) << "gl_manager::gl_manager()";
    if (nullptr != api::instance().glfw_set_error_callback(gl_manager::error_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous error callback";
    }

    if (GLFW_TRUE != api::instance().glfw_init()) {
        BOOST_LOG_TRIVIAL(trace) << "gl_manager::gl_manager() glfw_error";
        throw glfw_error("glfwInit() failed");
    }

    api::instance().glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    api::instance().glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    api::instance().glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    BOOST_LOG_TRIVIAL(info) << "gl_manager started successfully";
    BOOST_LOG_TRIVIAL(trace) << "gl_manager::gl_manager() end";
}

gl_manager::~gl_manager() {
    BOOST_LOG_TRIVIAL(debug) << "gl_manager::gl_managerr()";
    api::instance().glfw_terminate();
    BOOST_LOG_TRIVIAL(trace) << "gl_manager::gl_managerr() end";
}

gl_manager &gl_manager::instance() {
    static gl_manager static_instance;
    return static_instance;
}

} // namespace opengl_wrapper
