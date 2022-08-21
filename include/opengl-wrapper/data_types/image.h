#ifndef OPENGL_WRAPPER_IMAGE_H
#define OPENGL_WRAPPER_IMAGE_H

#include <filesystem>

namespace opengl_wrapper {

class image {
  public:
    /**
     * @brief Creates the image object while loading it from the filesystem.
     * @param path
     */
    explicit image(const std::filesystem::path &path);

    /**
     * Frees STB buffer.
     */
    ~image();

    /**
     * @brief Image move constructor.
     * @param other Image to be emptied.
     */
    image(image &&other) noexcept;

    /**
     * @brief Image move-assignment operator.
     * @param other Image to be emptied.
     * @return Reference to this.
     */
    image &operator=(image &&other) noexcept;

    image(const image &) = delete;
    image &operator=(const image &) = delete;

    /**
     * @brief Gets the image width.
     * @return Image width.
     */
    [[nodiscard]] int get_width() const;

    /**
     * @brief Gets the image height.
     * @return Image height.
     */
    [[nodiscard]] int get_height() const;

    /**
     * @brief Gets the image data.
     * @return Image data.
     */
    [[nodiscard]] const unsigned char *get_data() const;

    /**
     * @brief Informs whether or not the image has an alpha channel.
     * @return true if it has an alpha channel.
     */
    [[nodiscard]] bool has_alpha() const;

  private:
    int m_width{};
    int m_height{};
    int m_num_channels{};
    unsigned char *m_data{};
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_IMAGE_H
