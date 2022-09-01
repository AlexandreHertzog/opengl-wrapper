#include "opengl-wrapper/graphics/ogl_graphics.h"

#include "data_types/buffer.h"
#include "data_types/program.h"
#include "data_types/shader.h"
#include "data_types/texture.h"
#include "data_types/vertex_array.h"
#include "utils/formatter.h"
#include "utils/glfw_error.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

ogl_graphics::ogl_graphics()
    : m_error_handler([](int error_code, // NOLINT(cert-err58-cpp)
                         const char *description) {
          BOOST_LOG_TRIVIAL(error) << "GLFW error: error_code=" << formatter::format_glfw_error_code(error_code)
                                   << ", description=\"" << description << "\"";
      }) {

    BOOST_LOG_TRIVIAL(debug) << "graphics::graphics";
    if (nullptr != glfw_set_error_callback(m_error_handler)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous error callback";
    }

    if (GLFW_TRUE != glfw_init()) {
        throw glfw_error("glfwInit() failed");
    }

    glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

ogl_graphics::~ogl_graphics() {
    glfw_terminate();
}

void ogl_graphics::activate(const texture &tex) {
    glActiveTexture(tex.get_unit());
}

void ogl_graphics::attach_shader(const program &p, const shader &s) {
    glAttachShader(p.get_id(), s.get_id());
}

void ogl_graphics::bind(const buffer &b) {
    glBindBuffer(static_cast<GLenum>(b.get_target()), b.get_id());
}

void ogl_graphics::bind(const texture &t) {
    glBindTexture(static_cast<GLenum>(t.get_target()), t.get_id());
}

void ogl_graphics::bind(const vertex_array &va) {
    glBindVertexArray(va.get_id());
}

void ogl_graphics::buffer_data(const buffer &b, size_t size, const void *data) {
    glBufferData(static_cast<GLenum>(b.get_target()), size, data, GL_STATIC_DRAW);
}

void ogl_graphics::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ogl_graphics::set_clear_color(const color_alpha_t &c) {
    glClearColor(c[0], c[1], c[2], c[3]);
}

void ogl_graphics::compile(const shader &s) {
    glCompileShader(s.get_id());
}

identifier_t ogl_graphics::new_program() {
    return glCreateProgram();
}

identifier_t ogl_graphics::new_shader(shader_type_t type) {
    return glCreateShader(static_cast<GLenum>(type));
}

void ogl_graphics::delete_buffers(size_t n, const identifier_t *buffers) {
    glDeleteBuffers(n, buffers);
}

void ogl_graphics::delete_program(identifier_t program) {
    glDeleteProgram(program);
}

void ogl_graphics::delete_shader(identifier_t shader) {
    glDeleteShader(shader);
}

void ogl_graphics::delete_textures(size_t n, const identifier_t *textures) {
    glDeleteTextures(n, textures);
}

void ogl_graphics::delete_vertex_arrays(size_t n, const identifier_t *arrays) {
    glDeleteVertexArrays(n, arrays);
}

void ogl_graphics::disable(opengl_wrapper::graphics_feature_t cap) {
    glDisable(static_cast<GLenum>(cap));
}

void ogl_graphics::draw_arrays(int first, size_t count) {
    glDrawArrays(GL_TRIANGLES, first, count);
}

void ogl_graphics::draw_elements(const std::vector<unsigned> &indices) {
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void ogl_graphics::enable(graphics_feature_t cap) {
    glEnable(static_cast<GLenum>(cap));
}

void ogl_graphics::enable_vertex_attrib_array(unsigned index) {
    glEnableVertexAttribArray(index);
}

std::vector<identifier_t> ogl_graphics::new_buffers(size_t n) {
    std::vector<GLuint> ret(n);
    glGenBuffers(n, ret.data());
    return ret;
}

std::vector<GLuint> ogl_graphics::new_textures(size_t n) {
    std::vector<GLuint> ret(n);
    glGenTextures(n, ret.data());
    return ret;
}

std::vector<identifier_t> ogl_graphics::new_vertex_arrays(size_t amount) {
    std::vector<identifier_t> ret(amount);
    glGenVertexArrays(amount, ret.data());
    return ret;
}

void ogl_graphics::generate_mipmap(const texture &t) {
    glGenerateMipmap(static_cast<GLenum>(t.get_target()));
}

std::string ogl_graphics::get_info_log(const program &p) {
    constexpr auto length = 512;
    std::array<GLchar, length> log = {'\0'};
    glGetProgramInfoLog(p.get_id(), log.size(), nullptr, log.data());
    return log.data();
}

int ogl_graphics::get_parameter(const program &p, program_parameter_t param) {
    int ret = 0;
    glGetProgramiv(p.get_id(), static_cast<GLenum>(param), &ret);
    return ret;
}

std::string ogl_graphics::get_info_log(const shader &s) {
    constexpr auto length = 512;
    std::array<GLchar, length> log = {'\0'};
    glGetShaderInfoLog(s.get_id(), log.size(), nullptr, log.data());
    return log.data();
}

int ogl_graphics::get_parameter(const shader &s, shader_parameter_t param) {
    int ret = 0;
    glGetShaderiv(s.get_id(), static_cast<GLenum>(param), &ret);
    return ret;
}

int ogl_graphics::get_uniform_location(const program &p, const char *name) {
    return glGetUniformLocation(p.get_id(), name);
}

void ogl_graphics::link(const program &p) {
    glLinkProgram(p.get_id());
}

void ogl_graphics::polygon_mode(polygon_mode_t mode) {
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
}

void ogl_graphics::set_sources(const shader &s, size_t num_sources, const char **sources) {
    glShaderSource(s.get_id(), num_sources, sources, nullptr);
}

void ogl_graphics::set_image(size_t width, size_t height, texture_format_t format, const unsigned char *data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, static_cast<GLenum>(format), GL_UNSIGNED_BYTE, data);
}

void ogl_graphics::set_parameter(texture_parameter_t name, texture_parameter_values_t value) {
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(name), static_cast<GLint>(value));
}

void ogl_graphics::set_uniform(int location, float v0) {
    glUniform1f(location, v0);
}

void ogl_graphics::set_uniform(int location, GLint v0) {
    glUniform1i(location, v0);
}

void ogl_graphics::set_uniform(int location, const std::array<float, 3> &v) {
    glUniform3f(location, v[0], v[1], v[2]);
}

void ogl_graphics::set_uniform(int location, const std::array<float, 4> &v) {
    glUniform4f(location, v[0], v[1], v[2], v[3]);
}

void ogl_graphics::set_matrix4_uniform(int location, size_t count, const float *value) {
    glUniformMatrix4fv(location, count, GL_FALSE, value);
}

void ogl_graphics::use(const program &p) {
    glUseProgram(p.get_id());
}

void ogl_graphics::vertex_attrib_pointer(unsigned index, size_t size, size_t stride, unsigned offset) {
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(offset));
}

void ogl_graphics::set_viewport(size_t width, size_t height) {
    glViewport(0, 0, width, height);
}

GLFWwindow *ogl_graphics::glfw_create_window(int width, int height, const char *title, GLFWmonitor *monitor,
                                             GLFWwindow *share) {
    return glfwCreateWindow(width, height, title, monitor, share);
}

void ogl_graphics::glfw_destroy_window(GLFWwindow *window) {
    glfwDestroyWindow(window);
}

int ogl_graphics::glfw_init() {
    return glfwInit();
}

void ogl_graphics::glfw_make_context_current(GLFWwindow *window) {
    glfwMakeContextCurrent(window);
}

void ogl_graphics::glfw_poll_events() {
    glfwPollEvents();
}

GLFWcursorposfun ogl_graphics::glfw_set_cursor_pos_callback(GLFWwindow *window, GLFWcursorposfun fun) {
    return glfwSetCursorPosCallback(window, fun);
}

GLFWerrorfun ogl_graphics::glfw_set_error_callback(GLFWerrorfun callback) {
    return glfwSetErrorCallback(callback);
}

GLFWframebuffersizefun ogl_graphics::glfw_set_framebuffer_size_callback(GLFWwindow *window,
                                                                        GLFWframebuffersizefun cbfun) {
    return glfwSetFramebufferSizeCallback(window, cbfun);
}

void ogl_graphics::glfw_set_input_mode(GLFWwindow *window, int mode, int value) {
    glfwSetInputMode(window, mode, value);
}

GLFWkeyfun ogl_graphics::glfw_set_key_callback(GLFWwindow *window, GLFWkeyfun cbfun) {
    return glfwSetKeyCallback(window, cbfun);
}

void ogl_graphics::glfw_set_window_should_close(GLFWwindow *window, int value) {
    glfwSetWindowShouldClose(window, value);
}

void ogl_graphics::glfw_swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void ogl_graphics::glfw_terminate() {
    glfwTerminate();
}

void ogl_graphics::glfw_window_hint(int hint, int value) {
    glfwWindowHint(hint, value);
}

int ogl_graphics::glfw_window_should_close(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

} // namespace opengl_wrapper
