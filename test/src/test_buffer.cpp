#include "mock_graphics.h"

#include "game-engine/data_types/buffer.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::A;
using ::testing::Exactly;
using ::testing::Return;

TEST(BufferTest, buffer_build_2) {
    const std::vector<GLuint> ret = {1, 2};

    game_engine_test::mock_graphics graphics;

    EXPECT_CALL(graphics, gl_gen_buffers(2)).Times(Exactly(1)).WillOnce(Return(ret));
    EXPECT_CALL(graphics, gl_delete_buffers(1, A<const GLuint *>())).Times(Exactly(2));

    auto buffers = game_engine::buffer_t::build(2);
    EXPECT_EQ(buffers.size(), 2);
    EXPECT_EQ(buffers[0].get_id(), ret[0]);
    EXPECT_EQ(buffers[1].get_id(), ret[1]);
}

TEST(BufferTest, buffer_build_0) {
    game_engine_test::mock_graphics graphics;
    EXPECT_DEATH(game_engine::buffer_t::build(0), "amount > 0");
}

TEST(BufferTest, buffer_build_default) {
    const std::vector<GLuint> ret = {1};

    game_engine_test::mock_graphics graphics;

    EXPECT_CALL(graphics, gl_gen_buffers(1)).Times(Exactly(1)).WillOnce(Return(ret));
    EXPECT_CALL(graphics, gl_delete_buffers(1, A<const GLuint *>())).Times(Exactly(1));

    auto buffers = game_engine::buffer_t();
    EXPECT_EQ(buffers.get_id(), ret[0]);
}

TEST(BufferTest, buffer_move_constructor) {
    game_engine_test::mock_graphics graphics;
    game_engine::buffer_t source(1, 2);

    EXPECT_CALL(graphics, gl_gen_buffers(_)).Times(Exactly(0));
    EXPECT_CALL(graphics, gl_delete_buffers(_, _)).Times(Exactly(1));

    game_engine::buffer_t dest(std::move(source));
    EXPECT_EQ(dest.get_id(), 1);
    EXPECT_EQ(dest.get_target(), 2);
    EXPECT_EQ(source.get_id(), 0);
    EXPECT_EQ(source.get_target(), 0);
}

TEST(BufferTest, buffer_move_assignment_operator) {
    game_engine_test::mock_graphics graphics;
    game_engine::buffer_t source(1, 2);

    EXPECT_CALL(graphics, gl_gen_buffers(_)).Times(Exactly(0));
    EXPECT_CALL(graphics, gl_delete_buffers(_, _)).Times(Exactly(1));

    game_engine::buffer_t dest(1);
    dest = std::move(source);
    EXPECT_EQ(dest.get_id(), 1);
    EXPECT_EQ(dest.get_target(), 2);
    EXPECT_EQ(source.get_id(), 0);
    EXPECT_EQ(source.get_target(), 0);
}

TEST(BufferTest, buffer_bind) {
    const std::vector<GLuint> ret = {1};

    game_engine_test::mock_graphics graphics;

    EXPECT_CALL(graphics, gl_bind_buffer(2, 1)).Times(Exactly(1));
    EXPECT_CALL(graphics, gl_delete_buffers(1, A<const GLuint *>())).Times(Exactly(1));

    auto buffer = game_engine::buffer_t(1, 2);
    buffer.bind();
}
