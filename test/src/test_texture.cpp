#include "mock_graphics.h"
#include "opengl-wrapper/data_types/texture.h"
#include "gtest/gtest.h"

using testing::A;
using testing::Exactly;
using testing::Return;

TEST(TextureTest, texture_constructor_gen_textures) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(1)).WillOnce(Return(ids));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, target);
    EXPECT_EQ(t1.get_id(), ids[0]);
}

TEST(TextureTest, texture_constructor_no_gen_textures) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(0));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t(unit, target, ids[0]);
    EXPECT_EQ(t.get_id(), ids[0]);
}

TEST(TextureTest, texture_constructor_invalid_values) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(0));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(0));

    std::unique_ptr<opengl_wrapper::texture> t;
    EXPECT_DEATH(t.reset(new opengl_wrapper::texture(unit, 0)), "0 != target");
    EXPECT_DEATH(t.reset(new opengl_wrapper::texture(0, target)), "0 != unit");
    EXPECT_DEATH(t.reset(new opengl_wrapper::texture(0, 0)), "0 != target");
}

TEST(TextureTest, texture_move_constructor) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(0));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, target, ids[0]);
    opengl_wrapper::texture t2(std::move(t1));
    EXPECT_EQ(t1.get_id(), 0);
    EXPECT_EQ(t1.get_target(), 0);
    EXPECT_EQ(t1.get_unit(), 0);
    EXPECT_EQ(t2.get_id(), ids[0]);
    EXPECT_EQ(t2.get_target(), target);
    EXPECT_EQ(t2.get_unit(), unit);
}

TEST(TextureTest, texture_move_assignment_operator) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit1 = 1;
    constexpr int unit2 = 2;
    constexpr GLenum target1 = 3;
    constexpr GLenum target2 = 4;
    const std::vector<GLuint> ids1 = {5};
    const std::vector<GLuint> ids2 = {6};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(2)).WillOnce(Return(ids1)).WillOnce(Return(ids2));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(2));

    opengl_wrapper::texture t1(unit1, target1);
    opengl_wrapper::texture t2(unit2, target2);

    t2 = std::move(t1);

    EXPECT_EQ(t1.get_id(), 0);
    EXPECT_EQ(t1.get_target(), 0);
    EXPECT_EQ(t1.get_unit(), 0);
    EXPECT_EQ(t2.get_id(), ids1[0]);
    EXPECT_EQ(t2.get_target(), target1);
    EXPECT_EQ(t2.get_unit(), unit1);
}

TEST(TextureTest, texture_set_parameter) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};
    constexpr auto pname = 4;
    constexpr auto param = 5;

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(1)).WillOnce(Return(ids));
    EXPECT_CALL(graphics, gl_activate_texture(unit)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_bind_texture(target, ids[0])).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_parameter_i(target, pname, param)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, target);
    t1.bind();
    t1.set_parameter(pname, param);
    EXPECT_EQ(t1.get_id(), ids[0]);
}

TEST(TextureTest, texture_set_image) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};
    constexpr GLint level = 4;
    constexpr GLint internalformat = 5;
    constexpr GLsizei width = 6;
    constexpr GLsizei height = 7;
    constexpr GLint border = 8;
    constexpr GLenum format = 9;
    constexpr GLenum type = 10;
    const void *data = &graphics;

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(1)).WillOnce(Return(ids));
    EXPECT_CALL(graphics, gl_activate_texture(unit)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_bind_texture(target, ids[0])).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_image_2d(target, level, internalformat, width, height, border, format, type, data))
        .Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, target);
    t1.bind();
    t1.set_image(level, internalformat, width, height, border, format, type, data);
    EXPECT_EQ(t1.get_id(), ids[0]);
}

TEST(TextureTest, texture_generate_mipmap) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = 2;
    const std::vector<GLuint> ids = {3};

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(1)).WillOnce(Return(ids));
    EXPECT_CALL(graphics, gl_activate_texture(unit)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_bind_texture(target, ids[0])).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_generate_mipmap(target)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, target);
    t1.bind();
    t1.generate_mipmap();
    EXPECT_EQ(t1.get_id(), ids[0]);
}

TEST(TextureTest, texture_set_image_from_path) {
    opengl_wrapper_test::mock_graphics graphics;

    constexpr int unit = 1;
    constexpr GLenum target = GL_TEXTURE_2D;
    const std::vector<GLuint> ids = {3};

    const std::filesystem::path image_path("./sample.png");
    constexpr GLsizei width = 20;
    constexpr GLsizei height = 18;

    EXPECT_CALL(graphics, gl_gen_textures(1)).Times(Exactly(1)).WillOnce(Return(ids));
    EXPECT_CALL(graphics, gl_activate_texture(unit)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_bind_texture(target, ids[0])).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_parameter_i(target, GL_TEXTURE_WRAP_S, GL_REPEAT)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_parameter_i(target, GL_TEXTURE_WRAP_T, GL_REPEAT)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_parameter_i(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_tex_parameter_i(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR)).Times(Exactly(1));
    EXPECT_CALL(graphics,
                gl_tex_image_2d(target, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, A<const void *>()));
    EXPECT_CALL(graphics, gl_generate_mipmap(target)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_textures(1, A<const GLuint *>())).Times(Exactly(1));

    opengl_wrapper::texture t1(unit, GL_TEXTURE_2D);
    t1.set_image_from_path(image_path);
    EXPECT_EQ(t1.get_id(), ids[0]);
}