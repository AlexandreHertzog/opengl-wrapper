#include "gl_manager.h"

#include <boost/log/trivial.hpp>

#include "exceptions/glfw_error.h"
#include "utils/formatter.h"

namespace opengl_wrapper {

const GLFWerrorfun GlManager::error_handler_ = [](int error_code, // NOLINT(cert-err58-cpp)
                                                  const char *description) {
    BOOST_LOG_TRIVIAL(error) << "GLFW error: error_code=" << Formatter::formatGlfwErrorCode(error_code)
                             << ", description=\"" << description << "\"";
};

GlManager::GlManager() {
    BOOST_LOG_TRIVIAL(debug) << "GlManager::GlManager()";
    if (nullptr != glfwSetErrorCallback(GlManager::error_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous error callback";
    }

    if (GLFW_TRUE != glfwInit()) {
        BOOST_LOG_TRIVIAL(trace) << "GlManager::GlManager() GlfwError";
        throw GlfwError("glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    BOOST_LOG_TRIVIAL(info) << "GlManager started successfully";
    BOOST_LOG_TRIVIAL(trace) << "GlManager::GlManager() end";
}

GlManager::~GlManager() {
    BOOST_LOG_TRIVIAL(debug) << "GlManager::~GlManager()";
    glfwTerminate();
    BOOST_LOG_TRIVIAL(trace) << "GlManager::~GlManager() end";
}

GlManager &GlManager::instance() {
    static GlManager static_instance;
    return static_instance;
}

} // namespace opengl_wrapper
