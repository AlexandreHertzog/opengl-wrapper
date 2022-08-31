#include "opengl-wrapper/graphics/ogl_graphics.h"

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

void ogl_graphics::gl_activate_texture(GLenum texture) {
    glActiveTexture(texture);
}

void ogl_graphics::gl_attach_shader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
}

void ogl_graphics::gl_bind_buffer(buffer_target_t target, GLuint buffer) {
    glBindBuffer(static_cast<GLenum>(target), buffer);
}

void ogl_graphics::gl_bind_texture(GLenum target, GLuint texture) {
    glBindTexture(target, texture);
}

void ogl_graphics::gl_bind_vertex_array(GLuint array) {
    glBindVertexArray(array);
}

void ogl_graphics::gl_buffer_data(buffer_target_t target, size_t size, const void *data) {
    glBufferData(static_cast<GLenum>(target), size, data, GL_STATIC_DRAW);
}

void ogl_graphics::gl_clear(GLbitfield mask) {
    glClear(mask);
}

void ogl_graphics::gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    glClearColor(red, green, blue, alpha);
}

void ogl_graphics::gl_compile_shader(GLuint shader) {
    glCompileShader(shader);
}

GLuint ogl_graphics::gl_create_program() {
    return glCreateProgram();
}

GLuint ogl_graphics::gl_create_shader(shader_type_t type) {
    return glCreateShader(static_cast<GLenum>(type));
}

void ogl_graphics::gl_delete_buffers(GLsizei n, const GLuint *buffers) {
    glDeleteBuffers(n, buffers);
}

void ogl_graphics::gl_delete_program(GLuint program) {
    glDeleteProgram(program);
}

void ogl_graphics::gl_delete_shader(GLuint shader) {
    glDeleteShader(shader);
}

void ogl_graphics::gl_delete_textures(GLsizei n, const GLuint *textures) {
    glDeleteTextures(n, textures);
}

void ogl_graphics::gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays) {
    glDeleteVertexArrays(n, arrays);
}

void ogl_graphics::gl_disable(GLenum cap) {
    glDisable(cap);
}

void ogl_graphics::gl_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
}

void ogl_graphics::gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    glDrawElements(mode, count, type, indices);
}

void ogl_graphics::gl_enable(GLenum cap) {
    glEnable(cap);
}

void ogl_graphics::gl_enable_vertex_attrib_array(GLuint index) {
    glEnableVertexAttribArray(index);
}

std::vector<identifier_t> ogl_graphics::gl_gen_buffers(size_t n) {
    std::vector<GLuint> ret(n);
    glGenBuffers(n, ret.data());
    return ret;
}

std::vector<GLuint> ogl_graphics::gl_gen_textures(GLsizei n) {
    std::vector<GLuint> ret(n);
    glGenTextures(n, ret.data());
    return ret;
}

void ogl_graphics::gl_gen_vertex_arrays(GLsizei n, GLuint *arrays) {
    glGenVertexArrays(n, arrays);
}

void ogl_graphics::gl_generate_mipmap(GLenum target) {
    glGenerateMipmap(target);
}

void ogl_graphics::gl_get_integerv(GLenum pname, GLint *data) {
    glGetIntegerv(pname, data);
}

void ogl_graphics::gl_get_program_info_log(GLuint program, GLsizei max_lenght, GLsizei *length, GLchar *info_log) {
    glGetProgramInfoLog(program, max_lenght, length, info_log);
}

void ogl_graphics::gl_get_programiv(GLuint program, GLenum pname, GLint *params) {
    glGetProgramiv(program, pname, params);
}

void ogl_graphics::gl_get_shader_info_log(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log) {
    glGetShaderInfoLog(shader, max_length, length, info_log);
}

void ogl_graphics::gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
    glGetShaderiv(shader, pname, params);
}

int ogl_graphics::gl_get_uniform_location(GLuint program, const GLchar *name) {
    return glGetUniformLocation(program, name);
}

void ogl_graphics::gl_link_program(GLuint program) {
    glLinkProgram(program);
}

void ogl_graphics::gl_polygon_mode(GLenum face, GLenum mode) {
    glPolygonMode(face, mode);
}

void ogl_graphics::gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
    glShaderSource(shader, count, string, length);
}

void ogl_graphics::gl_tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                                   GLint border, GLenum format, GLenum type, const void *data) {

    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
}

void ogl_graphics::gl_tex_parameter_i(GLuint texture, GLenum pname, GLint param) {
    glTexParameteri(texture, pname, param);
}

void ogl_graphics::gl_uniform(GLint location, GLfloat v0) {
    glUniform1f(location, v0);
}

void ogl_graphics::gl_uniform(GLint location, GLint v0) {
    glUniform1i(location, v0);
}

void ogl_graphics::gl_uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    glUniform3f(location, v0, v1, v2);
}

void ogl_graphics::gl_uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(location, v0, v1, v2, v3);
}

void ogl_graphics::gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4fv(location, count, transpose, value);
}

void ogl_graphics::gl_use_program(GLuint program) {
    glUseProgram(program);
}

void ogl_graphics::gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                            const void *pointer) {

    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void ogl_graphics::gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    glViewport(x, y, width, height);
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
