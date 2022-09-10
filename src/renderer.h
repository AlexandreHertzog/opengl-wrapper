#pragma once

#include <opengl-cpp/backend/gl.h>

namespace game_engine {

class shape_t;

class renderer_t {
  public:
    renderer_t(opengl_cpp::gl_t &gl);

    /**
     * @brief Draws a shape in the current viewport.
     * @param s Shape to be drawn.
     */
    void draw(shape_t &s);

    /**
     * @brief Sets the dimensions of the window viewport.
     * @param width Viewport width.
     * @param height Viewport height.
     */
    void set_viewport(size_t width, size_t height);

    /**
     * @brief Sets the wireframe mode the window.
     * @param enable Enable or disable wireframe mode.
     */
    void set_wireframe_mode(bool enable);

    /**
     * @brief Sets the depth test OpenGL feature.
     * @param enable Enable or disable depth test.
     */
    void set_depth_test(bool enable);

    /**
     * @brief Sets the background color.
     * @param c Color components.
     */
    void set_clear_color(const glm::vec4 &c);

    /**
     * @brief Clears the current viewport.
     */
    void clear();

  private:
    opengl_cpp::gl_t &m_gl;
};

} // namespace game_engine
