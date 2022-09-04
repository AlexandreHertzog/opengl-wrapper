#pragma once

#include "opengl-wrapper/graphics/graphics.h"
#include "gmock/gmock.h"

namespace opengl_wrapper_test {

class mock_graphics : opengl_wrapper::graphics_t {
  public:
    mock_graphics() {
        opengl_wrapper::graphics_t::set_instance(this);
    }

    MOCK_METHOD(void, gl_activate_texture, (GLenum texture), (override));
    MOCK_METHOD(void, gl_attach_shader, (GLuint program, GLuint shader), (override));
    MOCK_METHOD(void, gl_bind_buffer, (GLenum target, GLuint buffer), (override));
    MOCK_METHOD(void, gl_bind_texture, (GLenum target, GLuint texture), (override));
    MOCK_METHOD(void, gl_bind_vertex_array, (GLuint array), (override));
    MOCK_METHOD(void, gl_buffer_data, (GLenum target, GLsizeiptr size, const void *data, GLenum usage), (override));
    MOCK_METHOD(void, gl_clear, (GLbitfield mask), (override));
    MOCK_METHOD(void, gl_clear_color, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (override));
    MOCK_METHOD(void, gl_compile_shader, (GLuint shader), (override));
    MOCK_METHOD(GLuint, gl_create_program, (), (override));
    MOCK_METHOD(GLuint, gl_create_shader, (GLenum shader_type), (override));
    MOCK_METHOD(void, gl_delete_buffers, (GLsizei n, const GLuint *buffers), (override));
    MOCK_METHOD(void, gl_delete_program, (GLuint program), (override));
    MOCK_METHOD(void, gl_delete_shader, (GLuint shader), (override));
    MOCK_METHOD(void, gl_delete_textures, (GLsizei n, const GLuint *textures), (override));
    MOCK_METHOD(void, gl_delete_vertex_arrays, (GLsizei n, const GLuint *arrays), (override));
    MOCK_METHOD(void, gl_disable, (GLenum cap), (override));
    MOCK_METHOD(void, gl_draw_arrays, (GLenum mode, GLint first, GLsizei count), (override));
    MOCK_METHOD(void, gl_draw_elements, (GLenum mode, GLsizei count, GLenum type, const void *indices), (override));
    MOCK_METHOD(void, gl_enable, (GLenum cap), (override));
    MOCK_METHOD(void, gl_enable_vertex_attrib_array, (GLuint index), (override));
    MOCK_METHOD(std::vector<GLuint>, gl_gen_buffers, (GLsizei n), (override));
    MOCK_METHOD(std::vector<GLuint>, gl_gen_textures, (GLsizei n), (override));
    MOCK_METHOD(void, gl_gen_vertex_arrays, (GLsizei n, GLuint *arrays), (override));
    MOCK_METHOD(void, gl_generate_mipmap, (GLenum target), (override));
    MOCK_METHOD(void, gl_get_integerv, (GLenum pname, GLint *data), (override));
    MOCK_METHOD(void, gl_get_program_info_log, (GLuint program, GLsizei max_lenght, GLsizei *length, GLchar *info_log),
                (override));
    MOCK_METHOD(void, gl_get_programiv, (GLuint program, GLenum pname, GLint *params), (override));
    MOCK_METHOD(void, gl_get_shader_info_log, (GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log),
                (override));
    MOCK_METHOD(void, gl_get_shaderiv, (GLuint shader, GLenum pname, GLint *params), (override));
    MOCK_METHOD(int, gl_get_uniform_location, (GLuint program, const GLchar *name), (override));
    MOCK_METHOD(void, gl_link_program, (GLuint program), (override));
    MOCK_METHOD(void, gl_polygon_mode, (GLenum face, GLenum mode), (override));
    MOCK_METHOD(void, gl_shader_source, (GLuint shader, GLsizei count, const GLchar **string, const GLint *length),
                (override));
    MOCK_METHOD(void, gl_tex_image_2d,
                (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
                 GLenum format, GLenum type, const void *data),
                (override));
    MOCK_METHOD(void, gl_tex_parameter_i, (GLuint texture, GLenum pname, GLint param), (override));
    MOCK_METHOD(void, gl_uniform, (GLint location, GLfloat v0), (override));
    MOCK_METHOD(void, gl_uniform, (GLint location, GLint v0), (override));
    MOCK_METHOD(void, gl_uniform, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3), (override));
    MOCK_METHOD(void, gl_uniform_matrix_4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),
                (override));
    MOCK_METHOD(void, gl_use_program, (GLuint program), (override));
    MOCK_METHOD(void, gl_vertex_attrib_pointer,
                (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer),
                (override));
    MOCK_METHOD(void, gl_viewport, (GLint x, GLint y, GLsizei width, GLsizei height), (override));
    MOCK_METHOD(GLFWwindow *, glfw_create_window,
                (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share), (override));
    MOCK_METHOD(void, glfw_destroy_window, (GLFWwindow * window), (override));
    MOCK_METHOD(int, glfw_init, (), (override));
    MOCK_METHOD(void, glfw_make_context_current, (GLFWwindow * window), (override));
    MOCK_METHOD(void, glfw_poll_events, (), (override));
    MOCK_METHOD(GLFWcursorposfun, glfw_set_cursor_pos_callback, (GLFWwindow * window, GLFWcursorposfun fun),
                (override));
    MOCK_METHOD(GLFWerrorfun, glfw_set_error_callback, (GLFWerrorfun callback), (override));
    MOCK_METHOD(GLFWframebuffersizefun, glfw_set_framebuffer_size_callback,
                (GLFWwindow * window, GLFWframebuffersizefun cbfun), (override));
    MOCK_METHOD(void, glfw_set_input_mode, (GLFWwindow * window, int mode, int value), (override));
    MOCK_METHOD(GLFWkeyfun, glfw_set_key_callback, (GLFWwindow * window, GLFWkeyfun cbfun), (override));
    MOCK_METHOD(void, glfw_set_window_should_close, (GLFWwindow * window, int value), (override));
    MOCK_METHOD(void, glfw_swap_buffers, (GLFWwindow * window), (override));
    MOCK_METHOD(void, glfw_terminate, (), (override));
    MOCK_METHOD(void, glfw_window_hint, (int hint, int value), (override));
    MOCK_METHOD(int, glfw_window_should_close, (GLFWwindow * window), (override));
};

} // namespace opengl_wrapper_test
