#ifndef OPENGL_WRAPPER_API_H
#define OPENGL_WRAPPER_API_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <memory>

namespace opengl_wrapper {

class api {
  public:
    api(const api &) = delete;
    api(api &&) = delete;
    virtual ~api() = default;

    api &operator=(api &&) = delete;
    api &operator=(const api &) = delete;

    static api &instance();
    static void set_api(api *a);

    /**
     * @brief Attaches a shader object to a program object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
     * @param program Specifies the program object to which a shader object will be attached.
     * @param shader Specifies the shader object that is to be attached.
     */
    virtual void gl_attach_shader(GLuint program, GLuint shader);

    /**
     * @brief bind a named buffer object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
     * @param target Specifies the target to which the buffer object is bound, which must be one of the buffer binding
     * targets in the following table:
     * @param buffer Specifies the name of a buffer object.
     */
    virtual void gl_bind_buffer(GLenum target, GLuint buffer);

    /**
     * @brief bind a vertex array object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
     * @param array Specifies the name of the vertex array to bind
     */
    virtual void gl_bind_vertex_array(GLuint array);

    /**
     * @brief creates and initializes a buffer object's data store.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     * @param target Specifies the target to which the buffer object is bound for glBufferData, which must be one of the
     * buffer binding targets in the following table:
     * @param size Specifies the size in bytes of the buffer object's new data store.
     * @param data Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no
     * data is to be copied.
     * @param usage Specifies the expected usage pattern of the data store. The symbolic constant must be
     * GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW,
     * GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.
     */
    virtual void gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

    /**
     * @brief clear buffers to preset values.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClear.xhtml
     * @param mask Bitwise OR of masks that indicate the buffers to be cleared. The three masks are GL_COLOR_BUFFER_BIT,
     * GL_DEPTH_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.
     */
    virtual void gl_clear(GLbitfield mask);

    /**
     * @brief specify clear values for the color buffers.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClearColor.xhtml
     * @param red Specify the red, green, blue, and alpha values used when the color buffers are cleared. The initial
     * values are all 0.
     * @param green Specify the red, green, blue, and alpha values used when the color buffers are cleared. The initial
     * values are all 0.
     * @param blue Specify the red, green, blue, and alpha values used when the color buffers are cleared. The initial
     * values are all 0.
     * @param alpha Specify the red, green, blue, and alpha values used when the color buffers are cleared. The initial
     * values are all 0.
     */
    virtual void gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    /**
     * @brief Compiles a shader object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCompileShader.xhtml
     * @param shader Specifies the shader object to be compiled.
     */
    virtual void gl_compile_shader(GLuint shader);

    /**
     * @brief Creates a program object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateProgram.xhtml
     * @return an empty program object and returns a non-zero value by which it can be referenced. A program object is
     * an object to which shader objects can be attached. This provides a mechanism to specify the shader objects that
     * will be linked to create a program. It also provides a means for checking the compatibility of the shaders that
     * will be used to create a program (for instance, checking the compatibility between a vertex shader and a fragment
     * shader). When no longer needed as part of a program object, shader objects can be detached.
     */
    virtual GLuint gl_create_program();

    /**
     * @brief Creates a shader object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     * @param shaderType Specifies the type of shader to be created. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER,
     * GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
     * @return creates an empty shader object and returns a non-zero value by which it can be referenced. A shader
     * object is used to maintain the source code strings that define a shader. shaderType indicates the type of shader
     * to be created. Five types of shader are supported. A shader of type GL_COMPUTE_SHADER is a shader that is
     * intended to run on the programmable compute processor. A shader of type GL_VERTEX_SHADER is a shader that is
     * intended to run on the programmable vertex processor. A shader of type GL_TESS_CONTROL_SHADER is a shader that is
     * intended to run on the programmable tessellation processor in the control stage. A shader of type
     * GL_TESS_EVALUATION_SHADER is a shader that is intended to run on the programmable tessellation processor in the
     * evaluation stage. A shader of type GL_GEOMETRY_SHADER is a shader that is intended to run on the programmable
     * geometry processor. A shader of type GL_FRAGMENT_SHADER is a shader that is intended to run on the programmable
     * fragment processor.
     */
    virtual GLuint gl_create_shader(GLenum shaderType);

    /**
     * @brief delete named buffer objects.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
     * @param n Specifies the number of buffer objects to be deleted.
     * @param buffers Specifies an array of buffer objects to be deleted.
     */
    virtual void gl_delete_buffers(GLsizei n, const GLuint *buffers);

    /**
     * @brief Deletes a program object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
     * @param program Specifies the program object to be deleted.
     */
    virtual void gl_delete_program(GLuint program);

    /**
     * @brief Deletes a shader object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
     * @param shader Specifies the shader object to be deleted.
     */
    virtual void gl_delete_shader(GLuint shader);

    /**
     * @brief delete vertex array objects.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
     * @param n Specifies the number of vertex array objects to be deleted.
     * @param arrays Specifies the address of an array containing the n names of the objects to be deleted.
     */
    virtual void gl_delete_vertex_arrays(GLsizei n, const GLuint *arrays);

    /**
     * @brief render primitives from array data.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
     * @param mode Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP,
     * GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
     * GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
     * @param count Specifies the number of elements to be rendered.
     * @param type Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or
     * GL_UNSIGNED_INT.
     * @param indices Specifies a pointer to the location where the indices are stored.
     */
    virtual void gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices);

    /**
     * @brief Enable or disable a generic vertex attribute array
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
     * @param index Specifies the index of the generic vertex attribute to be enabled or disabled.
     */
    virtual void gl_enable_vertex_attrib_array(GLuint index);

    /**
     * @brief generate buffer object names
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
     * @param n Specifies the number of buffer object names to be generated.
     * @param buffers Specifies an array in which the generated buffer object names are stored.
     */
    virtual void gl_gen_buffers(GLsizei n, GLuint *buffers);

    /**
     * @brief generate vertex array object names
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     * @param n Specifies the number of vertex array object names to generate.
     * @param arrays Specifies an array in which the generated vertex array object names are stored.
     */
    virtual void gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);

    /**
     * @brief return the value or values of a selected parameter.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
     * @param pname Specifies the parameter value to be returned for non-indexed versions of glGet. The symbolic
     * constants in the list below are accepted.
     * @param data Returns the value or values of the specified parameter.
     */
    virtual void gl_get_integerv(GLenum pname, GLint *data);

    /**
     * @brief Returns the information log for a program object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetProgramInfoLog.xhtml
     * @param program Specifies the program object whose information log is to be queried.
     * @param maxLength Specifies the size of the character buffer for storing the returned information log.
     * @param length Returns the length of the string returned in infoLog (excluding the null terminator).
     * @param infoLog Specifies an array of characters that is used to return the information log.
     */
    virtual void gl_get_program_info_log(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

    /**
     * @brief Returns a parameter from a program object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetProgram.xhtml
     * @param program Specifies the program object to be queried.
     * @param pname Specifies the object parameter. Accepted symbolic names are GL_DELETE_STATUS, GL_LINK_STATUS,
     * GL_VALIDATE_STATUS, GL_INFO_LOG_LENGTH, GL_ATTACHED_SHADERS, GL_ACTIVE_ATOMIC_COUNTER_BUFFERS,
     * GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_BLOCKS,
     * GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, GL_ACTIVE_UNIFORM_MAX_LENGTH, GL_COMPUTE_WORK_GROUP_SIZE,
     * GL_PROGRAM_BINARY_LENGTH, GL_TRANSFORM_FEEDBACK_BUFFER_MODE, GL_TRANSFORM_FEEDBACK_VARYINGS,
     * GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, GL_GEOMETRY_VERTICES_OUT, GL_GEOMETRY_INPUT_TYPE, and
     * GL_GEOMETRY_OUTPUT_TYPE.
     * @param params Returns the requested object parameter.
     */
    virtual void gl_get_programiv(GLuint program, GLenum pname, GLint *params);

    /**
     * @brief Returns the information log for a shader object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetShaderInfoLog.xhtml
     * @param shader Specifies the shader object whose information log is to be queried.
     * @param maxLength Specifies the size of the character buffer for storing the returned information log.
     * @param length Returns the length of the string returned in infoLog (excluding the null terminator).
     * @param infoLog Specifies an array of characters that is used to return the information log.
     */
    virtual void gl_get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

    /**
     * @brief Returns a parameter from a shader object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetShader.xhtml
     * @param shader Specifies the shader object to be queried.
     * @param pname Specifies the object parameter. Accepted symbolic names are GL_SHADER_TYPE, GL_DELETE_STATUS,
     * GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH.
     * @param params Returns the requested object parameter.
     */
    virtual void gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params);

    /**
     * @brief Returns the location of a uniform variable
     * @param program Specifies the program object to be queried.
     * @param name Points to a null terminated string containing the name of the uniform variable whose location is to
     * be queried.
     * @return an integer that represents the location of a specific uniform variable within a program object.
     */
    virtual int gl_get_uniform_location(GLuint program, const GLchar *name);

    /**
     * @brief Links a program object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
     * @param program Specifies the handle of the program object to be linked.
     */
    virtual void gl_link_program(GLuint program);

    /**
     * @brief Replaces the source code in a shader object
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
     * @param shader Specifies the handle of the shader object whose source code is to be replaced.
     * @param count Specifies the number of elements in the string and length arrays.
     * @param string Specifies an array of pointers to strings containing the source code to be loaded into the shader.
     * @param length Specifies an array of string lengths.
     */
    virtual void gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);

    /**
     * @brief Specify the value of a uniform variable for the current program object.
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml
     * @param location Specifies the location of the uniform variable to be modified.
     * @param v0 For the scalar commands, specifies the new values to be used for the specified uniform variable.
     * @param v1 For the scalar commands, specifies the new values to be used for the specified uniform variable.
     * @param v2 For the scalar commands, specifies the new values to be used for the specified uniform variable.
     * @param v3 For the scalar commands, specifies the new values to be used for the specified uniform variable.
     */
    virtual void gl_uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    /**
     * @brief Installs a program object as part of current rendering state
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
     * @param program Specifies the handle of the program object whose executables are to be used as part of current
     * rendering state.
     */
    virtual void gl_use_program(GLuint program);

    /**
     * @brief define an array of generic vertex attribute data
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
     * @param index Specifies the index of the generic vertex attribute to be modified.
     * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally,
     * the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
     * @param type Specifies the data type of each component in the array. The symbolic constants GL_BYTE,
     * GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer
     * and glVertexAttribIPointer. Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV,
     * GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer.
     * GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for
     * that function. The initial value is GL_FLOAT.
     * @param normalized For glVertexAttribPointer, specifies whether fixed-point data values should be normalized
     * (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
     * @param stride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the
     * generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
     * @param pointer Specifies a offset of the first component of the first generic vertex attribute in the array in
     * the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
     */
    virtual void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                          const void *pointer);

    /**
     * @brief set the viewport
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
     * @param x Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0,0).
     * @param y Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0,0).
     * @param width Specify the width and height of the viewport. When a GL context is first attached to a window, width
     * and height are set to the dimensions of that window.
     * @param height Specify the width and height of the viewport. When a GL context is first attached to a window,
     * width and height are set to the dimensions of that window.
     */
    virtual void gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height);

    /**
     * @brief This function creates a window and its associated OpenGL or OpenGL ES context.
     * https://www.glfw.org/docs/3.3/group__window.html#ga3555a418df92ad53f917597fe2f64aeb
     * @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
     * @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
     * @param title The initial, UTF-8 encoded window title.
     * @param monitor The monitor to use for full screen mode, or NULL for windowed mode.
     * @param share The window whose context to share resources with, or NULL to not share resources.
     * @return The handle of the created window, or NULL if an error occurred.
     */
    virtual GLFWwindow *glfw_create_window(int width, int height, const char *title, GLFWmonitor *monitor,
                                           GLFWwindow *share);

    /**
     * @brief This function destroys the specified window and its context.
     * https://www.glfw.org/docs/3.3/group__window.html#gacdf43e51376051d2c091662e9fe3d7b2
     * @param window The window to destroy.
     */
    virtual void glfw_destroy_window(GLFWwindow *window);

    /**
     * @brief This function initializes the GLFW library.
     * https://www.glfw.org/docs/3.3/group__init.html#ga317aac130a235ab08c6db0834907d85e
     * @return GLFW_TRUE if successful, or GLFW_FALSE if an error occurred.
     */
    virtual int glfw_init();

    /**
     * @brief This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread.
     * @param window The window whose context to make current, or NULL to detach the current context.
     */
    virtual void glfw_make_context_current(GLFWwindow *window);

    /**
     * @brief This function processes only those events that are already in the event queue and then returns
     * immediately. https://www.glfw.org/docs/3.3/group__window.html#ga37bd57223967b4211d60ca1a0bf3c832
     */
    virtual void glfw_poll_events();

    /**
     * @brief This function sets the error callback, which is called with an error code and a human-readable description
     * each time a GLFW error occurs. https://www.glfw.org/docs/3.3/group__init.html#gaff45816610d53f0b83656092a4034f40
     * @param callback The new callback, or NULL to remove the currently set callback.
     * @return The previously set callback, or NULL if no callback was set.
     */
    virtual GLFWerrorfun glfw_set_error_callback(GLFWerrorfun callback);

    /**
     * @brief This function sets the framebuffer resize callback of the specified window, which is called when the
     * framebuffer of the specified window is resized.
     * https://www.glfw.org/docs/3.0/group__window.html#ga3203461a5303bf289f2e05f854b2f7cf
     * @param window The window whose callback to set.
     * @param cbfun The new callback, or NULL to remove the currently set callback.
     * @return The previously set callback, or NULL if no callback was set or an error occurred.
     */
    virtual GLFWframebuffersizefun glfw_set_framebuffer_size_callback(GLFWwindow *window, GLFWframebuffersizefun cbfun);

    /**
     * @brief This function sets the key callback of the specific window, which is called when a key is pressed,
     * repeated or released. https://www.glfw.org/docs/3.0/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
     * @param window The window whose callback to set.
     * @param cbfun The new key callback, or NULL to remove the currently set callback.
     * @return The previously set callback, or NULL if no callback was set or an error occurred.
     */
    virtual GLFWkeyfun glfw_set_key_callback(GLFWwindow *window, GLFWkeyfun cbfun);

    /**
     * @brief This function sets the value of the close flag of the specified window.
     * https://www.glfw.org/docs/3.3/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
     * @param window The window whose flag to change.
     * @param value The new value.
     */
    virtual void glfw_set_window_should_close(GLFWwindow *window, int value);

    /**
     * @brief This function swaps the front and back buffers of the specified window when rendering with OpenGL or
     * OpenGL ES. https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     * @param window The window whose buffers to swap.
     */
    virtual void glfw_swap_buffers(GLFWwindow *window);

    /**
     * @brief This function destroys all remaining windows and cursors, restores any modified gamma ramps and frees any
     * other allocated resources. https://www.glfw.org/docs/3.3/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
     */
    virtual void glfw_terminate();

    /**
     * @brief This function sets hints for the next call to glfwCreateWindow.
     * https://www.glfw.org/docs/3.3/group__window.html#ga7d9c8c62384b1e2821c4dc48952d2033
     * @param hint The window hint to set.
     * @param value The new value of the window hint.
     */
    virtual void glfw_window_hint(int hint, int value);

    /**
     * @brief This function returns the value of the close flag of the specified window.
     * @param window The window to query.
     * @return The value of the close flag.
     */
    virtual int glfw_window_should_close(GLFWwindow *window);

  protected:
    static std::unique_ptr<api> api_;

    api() = default;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_API_H
