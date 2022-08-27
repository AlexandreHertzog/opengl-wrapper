#pragma once

#include "opengl-wrapper/data_types/program.h"
#include "opengl-wrapper/data_types/texture.h"
#include "opengl-wrapper/graphics/graphics.h"
#include <cassert>
#include <functional>
#include <ostream>

namespace opengl_wrapper {

class window {
  public:
    using cursor_pos_cb_t = std::function<void(window &, double, double)>;
    using framebuffer_cb_t = std::function<void(window &, int, int)>;
    using key_cb_t = std::function<void(window &, int, int, int, int)>;

    /**
     * @brief Constructs a window object.
     * @param width window width.
     * @param height window height.
     * @param title window title.
     */
    window(int width, int height, const char *title);

    /**
     * @brief window move-constructor.
     * @param other window to be moved.
     */
    window(window &&other) noexcept;

    /**
     * @brief window destructor.
     */
    ~window();

    /**
     * @brief window move-assignment operator.
     * @param other window to be moved.
     * @return Reference to lhs.
     */
    window &operator=(window &&other) noexcept;

    window(const window &) = delete;
    window &operator=(const window &) = delete;

    /**
     * @brief window equality operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow refers to this object.
     */
    bool operator==(GLFWwindow *other) const;

    /**
     * @brief window difference operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow does not refer to this object.
     */
    bool operator!=(GLFWwindow *other) const;

    /**
     * @brief Makes the window context the one being referenced by OpenGL. See
     * https://www.glfw.org/docs/3.3/group__context.html#ga1c04dc242268f827290fe40aa1c91157
     */
    void set_as_context();

    /**
     * @brief Sets the window-resizing callback function. See
     * https://www.glfw.org/docs/3.0/group__window.html#ga3203461a5303bf289f2e05f854b2f7cf
     * @param fun Function to be called whenever the window is resized.
     */
    void set_framebuffer_callback(framebuffer_cb_t fun);

    /**
     * @brief Sets the key-pressed callback function. See
     * https://www.glfw.org/docs/3.0/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
     * @param fun Function to be called whenever a key is pressed.
     */
    void set_key_callback(key_cb_t fun);

    /**
     * @brief This function sets the cursor position callback of the specified window, which is called when the cursor
     * is moved. See https://www.glfw.org/docs/3.3/group__input.html#gac1f879ab7435d54d4d79bb469fe225d7
     * @param fun The new callback, or NULL to remove the currently set callback.
     */
    void set_cursor_pos_callback(cursor_pos_cb_t fun);

    /**
     * @brief Sets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
     * @param should_close 1 if the window should be closed, 0 otherwise.
     */
    void set_should_close(int should_close);

    /**
     * @brief Gets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
     * @return 1 if the window should be closed, 0 otherwise.
     */
    [[nodiscard]] bool get_should_close() const;

    /**
     * Swaps the front and the back buffers for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     */
    void swap_buffers();

    /**
     * @brief This function sets an input mode option for the specified window. See
     * https://www.glfw.org/docs/3.3/group__input.html#gaa92336e173da9c8834558b54ee80563b
     * @param mode One of GLFW_CURSOR, GLFW_STICKY_KEYS, GLFW_STICKY_MOUSE_BUTTONS, GLFW_LOCK_KEY_MODS or
     * GLFW_RAW_MOUSE_MOTION.
     * @param value The new value of the specified input mode.
     */
    void set_input_mode(int mode, int value);

    template <typename TYPE> void draw(TYPE &shapes) {
        graphics::instance().gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &shape : shapes) {
            assert(!shape.get_textures().empty());
            assert(shape.get_program());

            for (auto &t : shape.get_textures()) {
                assert(t);
                t->bind();
            }

            shape.get_program()->use(shape);
            shape.get_vertex_array().bind();

            const auto draw_order = shape.serialize_draw_order();
            if (!draw_order.empty()) {
                graphics::instance().gl_draw_elements(GL_TRIANGLES, draw_order.size(), GL_UNSIGNED_INT, nullptr);
            } else {
                graphics::instance().gl_draw_arrays(GL_TRIANGLES, 0, shape.serialize_vertices().size());
            }
        }

        swap_buffers();
        graphics::instance().glfw_poll_events();
    }

    /**
     * @brief Sets the dimensions of the window viewport.
     * @param width Viewport width.
     * @param height Viewport height.
     */
    void set_viewport(int width, int height);

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
     * @param red Red color component.
     * @param green Green component.
     * @param blue Blue component.
     * @param alpha Alpha component.
     */
    void set_clear_color(float red, float green, float blue, float alpha);

    [[nodiscard]] const GLFWwindow *get_window() const;

  private:
    GLFWwindow *m_window;

    cursor_pos_cb_t m_cursor_pos_callback;
    framebuffer_cb_t m_framebuffer_callback;
    key_cb_t m_key_callback;

    class manager;

    void register_callbacks();
};

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window &w);

} // namespace opengl_wrapper
