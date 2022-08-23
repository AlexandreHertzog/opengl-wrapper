#include "opengl-wrapper/graphics/graphics.h"

#include "utils/formatter.h"
#include "utils/glfw_error.h"

#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

graphics::graphics()
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
        BOOST_LOG_TRIVIAL(trace) << "graphics::graphics glfw_error";
        throw glfw_error("glfwInit() failed");
    }

    glfw_window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfw_window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfw_window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    BOOST_LOG_TRIVIAL(info) << "gl_manager started successfully";
}

graphics::~graphics() {
    BOOST_LOG_TRIVIAL(debug) << "graphics::graphics";
    glfw_terminate();
}

graphics &graphics::instance() {
    static graphics static_instance;
    return static_instance;
}

void graphics::gl_activate_texture(GLenum texture) {
    BOOST_LOG_TRIVIAL(trace) << "gl_activate_texture texture=" << texture;
    glActiveTexture(texture);
}

void graphics::gl_attach_shader(GLuint program, GLuint shader) {
    BOOST_LOG_TRIVIAL(trace) << "gl_attach_shader program=" << program << " shader=" << shader;
    glAttachShader(program, shader);
}

void graphics::gl_bind_buffer(GLenum target, GLuint buffer) {
    BOOST_LOG_TRIVIAL(trace) << "gl_bind_buffer target=" << target << " buffer=" << buffer;
    glBindBuffer(target, buffer);
}

void graphics::gl_bind_texture(GLenum target, GLuint texture) {
    BOOST_LOG_TRIVIAL(trace) << "gl_bind_texture target=" << target << " texture=" << texture;
    glBindTexture(target, texture);
}

void graphics::gl_bind_vertex_array(GLuint array) {
    BOOST_LOG_TRIVIAL(trace) << "gl_bind_vertex_array array=" << array;
    glBindVertexArray(array);
}

void graphics::gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    BOOST_LOG_TRIVIAL(trace) << "gl_buffer_data target=" << target << " size=" << size << " data=" << data
                             << " usage=" << usage;
    glBufferData(target, size, data, usage);
}

void graphics::gl_clear(GLbitfield mask) {
    BOOST_LOG_TRIVIAL(trace) << "gl_clear mask=" << mask;
    glClear(mask);
}

void graphics::gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    BOOST_LOG_TRIVIAL(trace) << "gl_clear_color red=" << red << " green=" << green << " blue=" << blue
                             << " alpha=" << alpha;
    glClearColor(red, green, blue, alpha);
}

void graphics::gl_compile_shader(GLuint shader) {
    BOOST_LOG_TRIVIAL(trace) << "gl_compile_shader shader=" << shader;
    glCompileShader(shader);
}

GLuint graphics::gl_create_program() {
    BOOST_LOG_TRIVIAL(trace) << "gl_create_program";
    return glCreateProgram();
}

GLuint graphics::gl_create_shader(GLenum shader_type) {
    BOOST_LOG_TRIVIAL(trace) << "gl_create_shader shader_type=" << shader_type;
    return glCreateShader(shader_type);
}

void graphics::gl_delete_buffers(GLsizei n, const GLuint *buffers) {
    BOOST_LOG_TRIVIAL(trace) << "gl_delete_buffers n=" << n << " buffers=" << buffers;
    glDeleteBuffers(n, buffers);
}

void graphics::gl_delete_program(GLuint program) {
    BOOST_LOG_TRIVIAL(trace) << "gl_delete_program program=" << program;
    glDeleteProgram(program);
}

void graphics::gl_delete_shader(GLuint shader) {
    BOOST_LOG_TRIVIAL(trace) << "gl_delete_shader shader=" << shader;
    glDeleteShader(shader);
}

void graphics::gl_delete_textures(GLsizei n, const GLuint *textures) {
    BOOST_LOG_TRIVIAL(trace) << "gl_delete_textures n=" << n << " textures=" << textures;
    glDeleteTextures(n, textures);
}

void graphics::gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays) {
    BOOST_LOG_TRIVIAL(trace) << "gl_delete_vertex_arrays n=" << n << " arrays=" << arrays;
    glDeleteVertexArrays(n, arrays);
}

void graphics::gl_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    BOOST_LOG_TRIVIAL(trace) << "gl_draw_arrays mode=" << mode << " first=" << first << " count=" << count;
    glDrawArrays(mode, first, count);
}

void graphics::gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    BOOST_LOG_TRIVIAL(trace) << "gl_draw_elements mode=" << mode << " count=" << count << " type=" << type
                             << " indices=" << indices;
    glDrawElements(mode, count, type, indices);
}

void graphics::gl_enable(GLenum cap) {
    BOOST_LOG_TRIVIAL(trace) << "gl_enable cap=" << cap;
    glEnable(cap);
}

void graphics::gl_enable_vertex_attrib_array(GLuint index) {
    BOOST_LOG_TRIVIAL(trace) << "gl_enable_vertex_attrib_array index=" << index;
    glEnableVertexAttribArray(index);
}

void graphics::gl_gen_buffers(GLsizei n, GLuint *buffers) {
    BOOST_LOG_TRIVIAL(trace) << "gl_gen_buffers n=" << n << " buffers=" << buffers;
    glGenBuffers(n, buffers);
}

void graphics::gl_gen_textures(GLsizei n, GLuint *textures) {
    BOOST_LOG_TRIVIAL(trace) << "gl_gen_textures n=" << n << " textures=" << textures;
    glGenTextures(n, textures);
}

void graphics::gl_gen_vertex_arrays(GLsizei n, GLuint *arrays) {
    BOOST_LOG_TRIVIAL(trace) << "gl_gen_vertex_arrays n=" << n << " arrays=" << arrays;
    glGenVertexArrays(n, arrays);
}

void graphics::gl_generate_mipmap(GLenum target) {
    BOOST_LOG_TRIVIAL(trace) << "gl_generate_mipmap target=" << target;
    glGenerateMipmap(target);
}

void graphics::gl_get_integerv(GLenum pname, GLint *data) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_integerv pname=" << pname << " data=" << data;
    glGetIntegerv(pname, data);
}

void graphics::gl_get_program_info_log(GLuint program, GLsizei max_lenght, GLsizei *length, GLchar *info_log) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_program_info_log program=" << program << " max_length=" << max_lenght
                             << " length=" << length << " info_log=" << info_log;
    glGetProgramInfoLog(program, max_lenght, length, info_log);
}

void graphics::gl_get_programiv(GLuint program, GLenum pname, GLint *params) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_programiv program=" << program << " pname=" << pname << " params=" << params;
    glGetProgramiv(program, pname, params);
}

void graphics::gl_get_shader_info_log(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_shader_info_log program=" << shader << " max_length=" << max_length
                             << " length=" << length << " info_log=" << info_log;
    glGetShaderInfoLog(shader, max_length, length, info_log);
}

void graphics::gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_shaderiv shader=" << shader << " pname=" << pname << " params=" << params;
    glGetShaderiv(shader, pname, params);
}

int graphics::gl_get_uniform_location(GLuint program, const GLchar *name) {
    BOOST_LOG_TRIVIAL(trace) << "gl_get_uniform_location program=" << program << " name=" << name;
    return glGetUniformLocation(program, name);
}

void graphics::gl_link_program(GLuint program) {
    BOOST_LOG_TRIVIAL(trace) << "gl_link_program program=" << program;
    glLinkProgram(program);
}

void graphics::gl_polygon_mode(GLenum face, GLenum mode) {
    BOOST_LOG_TRIVIAL(trace) << "gl_polygon_mode face=" << face << " mode=" << mode;
    glPolygonMode(face, mode);
}

void graphics::gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
    BOOST_LOG_TRIVIAL(trace) << "gl_shader_source shader=" << shader << " count=" << count << " string=" << string
                             << " length=" << length;
    glShaderSource(shader, count, string, length);
}

void graphics::gl_tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                               GLint border, GLenum format, GLenum type, const void *data) {

    BOOST_LOG_TRIVIAL(trace) << "gl_tex_image_2d target=" << target << " level=" << level
                             << " internalformat=" << internalformat << " width=" << width << " height=" << height
                             << " border=" << border << " format=" << format << " type=" << type << " data=" << data;

    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
}

void graphics::gl_tex_parameter_i(GLuint texture, GLenum pname, GLint param) {
    BOOST_LOG_TRIVIAL(trace) << "gl_tex_parameter_i texture=" << texture << " pname=" << pname << " param=" << param;
    glTexParameteri(texture, pname, param);
}

void graphics::gl_uniform1i(GLint location, GLint v0) {
    BOOST_LOG_TRIVIAL(trace) << "gl_uniform4f location=" << location << " v0=" << v0;
    glUniform1i(location, v0);
}

void graphics::gl_uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    BOOST_LOG_TRIVIAL(trace) << "gl_uniform4f location=" << location << " v0=" << v0 << " v1=" << v1 << " v2=" << v2
                             << " v3=" << v3;
    glUniform4f(location, v0, v1, v2, v3);
}

void graphics::gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    BOOST_LOG_TRIVIAL(trace) << "gl_uniform4fv location=" << location << " count=" << count << " value=" << value;
    glUniformMatrix4fv(location, count, transpose, value);
}

void graphics::gl_use_program(GLuint program) {
    BOOST_LOG_TRIVIAL(trace) << "gl_use_program program=" << program;
    glUseProgram(program);
}

void graphics::gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                        const void *pointer) {

    BOOST_LOG_TRIVIAL(trace) << "gl_vertex_attrib_pointer index=" << index << " size=" << size << " type=" << type
                             << " normalized=" << normalized << " string=" << stride << " pointer=" << pointer;
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void graphics::gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    BOOST_LOG_TRIVIAL(trace) << "gl_viewport x=" << x << " y=" << y << " width=" << width << " height=" << height;
    glViewport(x, y, width, height);
}

GLFWwindow *graphics::glfw_create_window(int width, int height, const char *title, GLFWmonitor *monitor,
                                         GLFWwindow *share) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_create_window width=" << width << " height=" << height << " title=" << title
                             << " monitor=" << monitor << " share=" << share;
    return glfwCreateWindow(width, height, title, monitor, share);
}

void graphics::glfw_destroy_window(GLFWwindow *window) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_destroy_window window=" << window;
    glfwDestroyWindow(window);
}

int graphics::glfw_init() {
    BOOST_LOG_TRIVIAL(trace) << "glfw_init";
    return glfwInit();
}

void graphics::glfw_make_context_current(GLFWwindow *window) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_make_context_current window=" << window;
    glfwMakeContextCurrent(window);
}

void graphics::glfw_poll_events() {
    BOOST_LOG_TRIVIAL(trace) << "glfw_poll_events";
    glfwPollEvents();
}

GLFWcursorposfun graphics::glfw_set_cursor_pos_callback(GLFWwindow *window, GLFWcursorposfun fun) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_cursor_pos_callback window=" << window << " fun=" << fun;
    return glfwSetCursorPosCallback(window, fun);
}

GLFWerrorfun graphics::glfw_set_error_callback(GLFWerrorfun callback) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_error_callback callback=" << callback;
    return glfwSetErrorCallback(callback);
}

GLFWframebuffersizefun graphics::glfw_set_framebuffer_size_callback(GLFWwindow *window, GLFWframebuffersizefun cbfun) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_framebuffer_size_callback window=" << window << " cbfun=" << cbfun;
    return glfwSetFramebufferSizeCallback(window, cbfun);
}

void graphics::glfw_set_input_mode(GLFWwindow *window, int mode, int value) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_input_mode window=" << window << " mode=" << mode << " value=" << value;
    glfwSetInputMode(window, mode, value);
}

GLFWkeyfun graphics::glfw_set_key_callback(GLFWwindow *window, GLFWkeyfun cbfun) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_key_callback window=" << window << " cbfun=" << cbfun;
    return glfwSetKeyCallback(window, cbfun);
}

void graphics::glfw_set_window_should_close(GLFWwindow *window, int value) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_set_window_should_close window=" << window << " value=" << value;
    glfwSetWindowShouldClose(window, value);
}

void graphics::glfw_swap_buffers(GLFWwindow *window) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_swap_buffers window=" << window;
    glfwSwapBuffers(window);
}

void graphics::glfw_terminate() {
    BOOST_LOG_TRIVIAL(trace) << "glfw_terminate";
    glfwTerminate();
}

void graphics::glfw_window_hint(int hint, int value) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_window_hint hint=" << hint << " value=" << value;
    glfwWindowHint(hint, value);
}

int graphics::glfw_window_should_close(GLFWwindow *window) {
    BOOST_LOG_TRIVIAL(trace) << "glfw_window_should_close window=" << window;
    return glfwWindowShouldClose(window);
}

} // namespace opengl_wrapper