#include "api.h"

namespace opengl_wrapper {

std::unique_ptr<api> api::api_;

api &api::instance() {
    if (nullptr == api_) {
        api_ = std::unique_ptr<api>(new api());
    }
    return *api_;
}

void api::set_api(api *a) {
    api_.reset(a);
}

void api::gl_attach_shader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
}

void api::gl_bind_buffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void api::gl_bind_vertex_array(GLuint array) {
    glBindVertexArray(array);
}

void api::gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void api::gl_clear(GLbitfield mask) {
    glClear(mask);
}

void api::gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    glClearColor(red, green, blue, alpha);
}

void api::gl_compile_shader(GLuint shader) {
    glCompileShader(shader);
}

GLuint api::gl_create_program() {
    return glCreateProgram();
}

GLuint api::gl_create_shader(GLenum shaderType) {
    return glCreateShader(shaderType);
}

void api::gl_delete_buffers(GLsizei n, const GLuint *buffers) {
    glDeleteBuffers(n, buffers);
}

void api::gl_delete_program(GLuint program) {
    glDeleteProgram(program);
}

void api::gl_delete_shader(GLuint shader) {
    glDeleteShader(shader);
}

void api::gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays) {
    glDeleteVertexArrays(n, arrays);
}

void api::gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    glDrawElements(mode, count, type, indices);
}

void api::gl_enable_vertex_attrib_array(GLuint index) {
    glEnableVertexAttribArray(index);
}

void api::gl_gen_buffers(GLsizei n, GLuint *buffers) {
    glGenBuffers(n, buffers);
}

void api::gl_gen_vertex_arrays(GLsizei n, GLuint *arrays) {
    glGenVertexArrays(n, arrays);
}

void api::gl_get_integerv(GLenum pname, GLint *data) {
    glGetIntegerv(pname, data);
}

void api::gl_get_program_info_log(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
    glGetProgramInfoLog(program, maxLength, length, infoLog);
}

void api::gl_get_programiv(GLuint program, GLenum pname, GLint *params) {
    glGetProgramiv(program, pname, params);
}

void api::gl_get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) {
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

void api::gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
    glGetShaderiv(shader, pname, params);
}

int api::gl_get_uniform_location(GLuint program, const GLchar *name) {
    return glGetUniformLocation(program, name);
}

void api::gl_link_program(GLuint program) {
    glLinkProgram(program);
}

void api::gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
    glShaderSource(shader, count, string, length);
}

void api::gl_uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(location, v0, v1, v2, v3);
}

void api::gl_use_program(GLuint program) {
    glUseProgram(program);
}

void api::gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                   const void *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void api::gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    glViewport(x, y, width, height);
}

GLFWwindow *api::glfw_create_window(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share) {
    return glfwCreateWindow(width, height, title, monitor, share);
}

void api::glfw_destroy_window(GLFWwindow *window) {
    glfwDestroyWindow(window);
}

int api::glfw_init() {
    return glfwInit();
}

void api::glfw_make_context_current(GLFWwindow *window) {
    glfwMakeContextCurrent(window);
}

void api::glfw_poll_events() {
    glfwPollEvents();
}

GLFWerrorfun api::glfw_set_error_callback(GLFWerrorfun callback) {
    return glfwSetErrorCallback(callback);
}

GLFWframebuffersizefun api::glfw_set_framebuffer_size_callback(GLFWwindow *window, GLFWframebuffersizefun cbfun) {
    return glfwSetFramebufferSizeCallback(window, cbfun);
}

GLFWkeyfun api::glfw_set_key_callback(GLFWwindow *window, GLFWkeyfun cbfun) {
    return glfwSetKeyCallback(window, cbfun);
}

void api::glfw_set_window_should_close(GLFWwindow *window, int value) {
    glfwSetWindowShouldClose(window, value);
}

void api::glfw_swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void api::glfw_terminate() {
    glfwTerminate();
}

void api::glfw_window_hint(int hint, int value) {
    glfwWindowHint(hint, value);
}

int api::glfw_window_should_close(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

} // namespace opengl_wrapper