#include "window.h"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"
#include "wraps/buffer.h"

namespace opengl_wrapper {

const char *Window::vshader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *Window::fshader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const float Window::vertices_[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                   0.0f,  0.0f,  0.5f, 0.0f};

Window::Window() {
    BOOST_LOG_TRIVIAL(debug) << "Window::Window()";

    resize_handler_ = [](GLFWwindow *window, int width, int height) {
        BOOST_LOG_TRIVIAL(debug)
            << "Window::resize_handler_(window=" << window
            << ", width=" << width << ", height=" << height << ")";

        glViewport(0, 0, width, height);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        BOOST_LOG_TRIVIAL(trace)
            << "Window::resize_handler_(window=" << window
            << ", width=" << width << ", height=" << height << ") end";
    };

    key_handler_ = [](GLFWwindow *glfw_window, int key, int scancode,
                      int action, int mods) {
        BOOST_LOG_TRIVIAL(debug)
            << "window::key_handler_(window=" << glfw_window << ", key=" << key
            << ", scancode=" << scancode << ", mods=" << mods << ")";

        auto &window = Window::instance();
        if (glfw_window != window.glfw_window_) {
            BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << glfw_window
                                     << " against " << window.glfw_window_;
            return;
        }

        const auto existing_action = window.action_map_.find(key);
        if (window.action_map_.end() != existing_action) {
            existing_action->second(action);
        }

        BOOST_LOG_TRIVIAL(trace)
            << "window::key_handler_(window=" << glfw_window << ", key=" << key
            << ", scancode=" << scancode << ", mods=" << mods << ") end";
    };

    setRefreshRate(60);
    BOOST_LOG_TRIVIAL(trace) << "Window::Window() end";
}

Window &Window::instance() {
    static Window static_instance;
    return static_instance;
}

void Window::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::init(width=" << width << ", height=" << height
        << ", title=" << title << ")";

    assert(!initialized_);

    glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (nullptr == glfw_window_) {
        BOOST_LOG_TRIVIAL(trace)
            << "Window::Window(width=" << width << ", height=" << height
            << ", title=" << title << ") GlfwError";
        throw GlfwError("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        BOOST_LOG_TRIVIAL(trace)
            << "Window::init(width=" << width << ", height=" << height
            << ", title=" << title << ") GladError";
        throw GladError("gladLoadGLLoader() failed");
    }

    if (glfwSetFramebufferSizeCallback(glfw_window_, resize_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    if (glfwSetKeyCallback(glfw_window_, key_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }

    glViewport(0, 0, width, height);
    vao_ = std::make_unique<VertexArrays>(1);
    vao_->bind(0);

    vbo_ = std::make_unique<Buffer>(1);
    vbo_->bind(0, GL_ARRAY_BUFFER);
    vbo_->buffer(sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    program_ = std::make_unique<Program>();
    program_->addShader(Shader(GL_VERTEX_SHADER, vshader_source));
    program_->addShader(Shader(GL_FRAGMENT_SHADER, fshader_source));
    program_->link();
    program_->use();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    initialized_ = true;

    BOOST_LOG_TRIVIAL(trace)
        << "Window::init(width=" << width << ", height=" << height
        << ", title=" << title << ") end";
}

void Window::renderLoop() noexcept {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ")";

    assert(initialized_);

    while (!glfwWindowShouldClose(glfw_window_)) {
        auto start_time = std::chrono::high_resolution_clock::now();

        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();

        std::chrono::duration<double, std::milli> loop_time_ms =
            std::chrono::high_resolution_clock::now() - start_time;

        BOOST_LOG_TRIVIAL(debug)
            << "renderLoop() frame_time_ms_=" << frame_time_ms_
            << ", loop_time_ms=" << loop_time_ms.count();

        if (loop_time_ms.count() < frame_time_ms_) {
            const auto wait_time_ms = frame_time_ms_ - loop_time_ms.count();
            BOOST_LOG_TRIVIAL(debug)
                << "Waiting " << wait_time_ms << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_ms) * 1000);
        } else {
            BOOST_LOG_TRIVIAL(debug)
                << "loop_time too large, skipping time filler";
        }
    }

    BOOST_LOG_TRIVIAL(trace)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ") end";
}

void Window::setKeyAction(int key, Action action) noexcept {
    auto existing_action = action_map_.find(key);
    if (action_map_.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        action_map_[key] = std::move(action);
    }
}

void Window::setWindowShouldClose(int value) noexcept {
    assert(initialized_);
    glfwSetWindowShouldClose(glfw_window_, value);
}

void Window::setRefreshRate(int refresh_rate) noexcept {
    assert(refresh_rate > 0);
    frame_time_ms_ = 1000.0 / refresh_rate;
}

} // namespace opengl_wrapper