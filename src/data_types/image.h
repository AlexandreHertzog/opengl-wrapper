#pragma once

#include <filesystem>

namespace game_engine {

class image_t {
  public:
    /**
     * @brief Creates the image object while loading it from the filesystem.
     * @param path
     */
    explicit image_t(const std::filesystem::path &path);

    /**
     * Frees STB buffer.
     */
    ~image_t();

    /**
     * @brief Image move constructor.
     * @param other Image to be emptied.
     */
    image_t(image_t &&other) noexcept;

    /**
     * @brief Image move-assignment operator.
     * @param other Image to be emptied.
     * @return Reference to this.
     */
    image_t &operator=(image_t &&other) noexcept;

    image_t(const image_t &) = delete;
    image_t &operator=(const image_t &) = delete;

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

} // namespace game_engine
