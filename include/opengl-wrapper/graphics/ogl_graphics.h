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

    // Allocators and deleters
    identifier_t new_program() override;
    identifier_t new_shader(shader_type_t type) override;
    std::vector<identifier_t> new_buffers(size_t n) override;
    std::vector<identifier_t> new_textures(size_t n) override;
    std::vector<identifier_t> new_vertex_arrays(size_t amount) override;
    void delete_buffers(size_t n, const identifier_t *buffers) override;
    void delete_program(identifier_t program) override;
    void delete_shader(identifier_t shader) override;
    void delete_textures(size_t n, const identifier_t *textures) override;
    void delete_vertex_arrays(size_t n, const identifier_t *arrays) override;

    // Texture functions
    void activate(const texture &tex) override;
    void bind(const texture &t) override;
    void generate_mipmap(const texture &t) override;
    void set_image(size_t width, size_t height, texture_format_t format, const unsigned char *data) override;
    void set_parameter(texture_parameter_t name, texture_parameter_values_t value) override;

    // Program functions
    void attach_shader(const program &p, const shader &s) override;
    std::string get_info_log(const program &p) override;
    int get_parameter(const program &p, program_parameter_t param) override;
    int get_uniform_location(const program &p, const char *name) override;
    void link(const program &p) override;
    void use(const program &p) override;
    void set_uniform(int location, float v0) override;
    void set_uniform(int location, int v0) override;
    void set_uniform(int location, const std::array<float, 3> &v) override;
    void set_uniform(int location, const std::array<float, 4> &v) override;
    void set_matrix4_uniform(int location, size_t count, const float *value) override;

    // Buffer functions
    void bind(const buffer &b) override;
    void buffer_data(const buffer &b, size_t size, const void *data) override;

    // Vertex array functions
    void bind(const vertex_array &va) override;
    void enable_vertex_attrib_array(unsigned index) override;
    void vertex_attrib_pointer(unsigned index, size_t size, size_t stride, unsigned offset) override;

    // Shader functions
    void compile(const shader &s) override;
    std::string get_info_log(const shader &s) override;
    int get_parameter(const shader &s, shader_parameter_t param) override;
    void set_sources(const shader &s, size_t num_sources, const char **sources) override;

    void clear() override;
    void set_clear_color(const color_alpha_t &c) override;
    void disable(graphics_feature_t cap) override;
    void draw_arrays(int first, size_t count) override;
    void draw_elements(const std::vector<unsigned> &indices) override;
    void enable(graphics_feature_t cap) override;
    void polygon_mode(polygon_mode_t mode) override;
    void set_viewport(size_t width, size_t height) override;

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
