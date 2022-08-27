#pragma once

#include "graphics.h"

namespace opengl_wrapper {

class ogl_graphics : public graphics {
  public:
    ogl_graphics();
    ~ogl_graphics() override;

    ogl_graphics(const ogl_graphics &) = delete;
    ogl_graphics(ogl_graphics &&) = delete;
    ogl_graphics &operator=(ogl_graphics &&) = delete;
    ogl_graphics &operator=(const ogl_graphics &) = delete;

    void gl_activate_texture(GLenum texture) override;
    void gl_attach_shader(GLuint program, GLuint shader) override;
    void gl_bind_buffer(GLenum target, GLuint buffer) override;
    void gl_bind_texture(GLenum target, GLuint texture) override;
    void gl_bind_vertex_array(GLuint array) override;
    void gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage) override;
    void gl_clear(GLbitfield mask) override;
    void gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) override;
    void gl_compile_shader(GLuint shader) override;
    GLuint gl_create_program() override;
    GLuint gl_create_shader(GLenum shader_type) override;
    void gl_delete_buffers(GLsizei n, const GLuint *buffers) override;
    void gl_delete_program(GLuint program) override;
    void gl_delete_shader(GLuint shader) override;
    void gl_delete_textures(GLsizei n, const GLuint *textures) override;
    void gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays) override;
    void gl_disable(GLenum cap) override;
    void gl_draw_arrays(GLenum mode, GLint first, GLsizei count) override;
    void gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) override;
    void gl_enable(GLenum cap) override;
    void gl_enable_vertex_attrib_array(GLuint index) override;
    std::vector<GLuint> gl_gen_buffers(GLsizei n) override;
    std::vector<GLuint> gl_gen_textures(GLsizei n) override;
    void gl_gen_vertex_arrays(GLsizei n, GLuint *arrays) override;
    void gl_generate_mipmap(GLenum target) override;
    void gl_get_integerv(GLenum pname, GLint *data) override;
    void gl_get_program_info_log(GLuint program, GLsizei max_lenght, GLsizei *length, GLchar *info_log) override;
    void gl_get_programiv(GLuint program, GLenum pname, GLint *params) override;
    void gl_get_shader_info_log(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log) override;
    void gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params) override;
    int gl_get_uniform_location(GLuint program, const GLchar *name) override;
    void gl_link_program(GLuint program) override;
    void gl_polygon_mode(GLenum face, GLenum mode) override;
    void gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) override;
    void gl_tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type, const void *data) override;
    void gl_tex_parameter_i(GLuint texture, GLenum pname, GLint param) override;
    void gl_uniform1i(GLint location, GLint v0) override;
    void gl_uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) override;
    void gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) override;
    void gl_use_program(GLuint program) override;
    void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                  const void *pointer) override;
    void gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height) override;
    GLFWwindow *glfw_create_window(int width, int height, const char *title, GLFWmonitor *monitor,
                                   GLFWwindow *share) override;
    void glfw_destroy_window(GLFWwindow *window) override;
    int glfw_init() override;
    void glfw_make_context_current(GLFWwindow *window) override;
    void glfw_poll_events() override;
    GLFWcursorposfun glfw_set_cursor_pos_callback(GLFWwindow *window, GLFWcursorposfun fun) override;
    GLFWerrorfun glfw_set_error_callback(GLFWerrorfun callback) override;
    GLFWframebuffersizefun glfw_set_framebuffer_size_callback(GLFWwindow *window,
                                                              GLFWframebuffersizefun cbfun) override;
    void glfw_set_input_mode(GLFWwindow *window, int mode, int value) override;
    GLFWkeyfun glfw_set_key_callback(GLFWwindow *window, GLFWkeyfun cbfun) override;
    void glfw_set_window_should_close(GLFWwindow *window, int value) override;
    void glfw_swap_buffers(GLFWwindow *window) override;
    void glfw_terminate() override;
    void glfw_window_hint(int hint, int value) override;
    int glfw_window_should_close(GLFWwindow *window) override;

  private:
    GLFWerrorfun m_error_handler;
};

} // namespace opengl_wrapper
