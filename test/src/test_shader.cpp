#include "mock_graphics.h"

#include "game-engine/data_types/shader.h"
#include "game-engine/utils/gl_error.h"
#include "gtest/gtest.h"

using testing::A;
using testing::Exactly;
using testing::Return;
using testing::SetArgPointee;

TEST(ShaderTest, shader_construct_compile_successfull) {
    game_engine_test::mock_graphics graphics;

    const char *source = "";
    constexpr auto id = 123;
    constexpr auto type = 234;

    EXPECT_CALL(graphics, gl_create_shader(type)).Times(Exactly(1)).WillOnce(Return(id));
    EXPECT_CALL(graphics, gl_shader_source(id, 1, A<const char **>(), nullptr)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_compile_shader(id)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_get_shaderiv(id, GL_COMPILE_STATUS, A<int *>()))
        .Times(Exactly(1))
        .WillOnce(SetArgPointee<2>(GL_TRUE));
    EXPECT_CALL(graphics, gl_delete_shader(id));

    game_engine::shader_t s(type, source);
    EXPECT_EQ(s.get_id(), id);
}

TEST(ShaderTest, shader_construct_no_compile) {
    game_engine_test::mock_graphics graphics;

    const char *source = nullptr;
    constexpr auto id = 123;
    constexpr auto type = 234;

    EXPECT_CALL(graphics, gl_create_shader(type)).Times(Exactly(1)).WillOnce(Return(id));
    EXPECT_CALL(graphics, gl_delete_shader(id));

    game_engine::shader_t s(type, source);
    EXPECT_EQ(s.get_id(), id);
}

TEST(ShaderTest, shader_construct_compile_failed) {
    game_engine_test::mock_graphics graphics;

    const char *source = "";
    constexpr auto id = 123;
    constexpr auto type = 234;

    EXPECT_CALL(graphics, gl_create_shader(type)).Times(Exactly(1)).WillOnce(Return(id));
    EXPECT_CALL(graphics, gl_shader_source(id, 1, A<const char **>(), nullptr)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_compile_shader(id)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_get_shaderiv(id, GL_COMPILE_STATUS, A<int *>()))
        .Times(Exactly(1))
        .WillOnce(SetArgPointee<2>(GL_FALSE));
    EXPECT_CALL(graphics, gl_get_shader_info_log(id, A<GLsizei>(), nullptr, A<char *>())).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_shader(id));

    std::unique_ptr<game_engine::shader_t> s;
    EXPECT_THROW(s.reset(new game_engine::shader_t(type, source)), game_engine::gl_error_t);
}

TEST(ShaderTest, shader_move_constructor) {
    game_engine_test::mock_graphics graphics;

    const char *source = nullptr;
    constexpr auto id = 123;
    constexpr auto type = 234;

    EXPECT_CALL(graphics, gl_create_shader(type)).Times(Exactly(1)).WillOnce(Return(id));
    EXPECT_CALL(graphics, gl_delete_shader(id));

    game_engine::shader_t s1(type, source);
    game_engine::shader_t s2(std::move(s1));

    EXPECT_EQ(s1.get_id(), 0);
    EXPECT_EQ(s2.get_id(), id);
}

TEST(ShaderTest, shader_move_assignment_operator) {
    game_engine_test::mock_graphics graphics;

    const char *source = nullptr;
    constexpr auto id1 = 1;
    constexpr auto id2 = 2;
    constexpr auto type1 = 3;
    constexpr auto type2 = 4;

    EXPECT_CALL(graphics, gl_create_shader(type1)).Times(Exactly(1)).WillOnce(Return(id1));
    EXPECT_CALL(graphics, gl_create_shader(type2)).Times(Exactly(1)).WillOnce(Return(id2));
    EXPECT_CALL(graphics, gl_delete_shader(id1));
    EXPECT_CALL(graphics, gl_delete_shader(id2));

    game_engine::shader_t s1(type1, source);
    game_engine::shader_t s2(type2, source);

    s2 = std::move(s1);

    EXPECT_EQ(s1.get_id(), 0);
    EXPECT_EQ(s2.get_id(), id1);
}

TEST(ShaderTest, shader_construct_from_path) {
    game_engine_test::mock_graphics graphics;

    const std::filesystem::path file_name = "./shader.vert";

    constexpr auto id = 1;
    constexpr auto type = 3;
    EXPECT_CALL(graphics, gl_create_shader(type)).Times(Exactly(1)).WillOnce(Return(id));
    EXPECT_CALL(graphics, gl_shader_source(id, 1, A<const char **>(), nullptr)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_compile_shader(id)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_get_shaderiv(id, GL_COMPILE_STATUS, A<int *>()))
        .Times(Exactly(1))
        .WillOnce(SetArgPointee<2>(GL_TRUE));
    EXPECT_CALL(graphics, gl_delete_shader(id));

    game_engine::shader_t s(type, file_name);
    EXPECT_EQ(s.get_id(), id);
}
