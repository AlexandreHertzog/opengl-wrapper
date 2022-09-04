#include "mock_graphics.h"
#include "opengl-wrapper/data_types/face.h"
#include "opengl-wrapper/parsers/obj_parser.h"
#include "gtest/gtest.h"

using testing::A;
using testing::Return;

TEST(obj_parser_test, parse) {
    using opengl_wrapper::operator==;

    opengl_wrapper::obj_parser_t parser("./untitled.obj");

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture_coords;
    std::vector<std::vector<opengl_wrapper::face_t>> faces;
    std::string name;
    bool smooth = true;

    while (parser.is_good()) {
        const auto type = parser.line_type();
        if (type == opengl_wrapper::obj_parser_t::line_type_t::object_name) {
            parser.get_line(name);
        } else if (type == opengl_wrapper::obj_parser_t::line_type_t::vertex) {
            parser.get_line(vertices);
        } else if (type == opengl_wrapper::obj_parser_t::line_type_t::face) {
            parser.get_line(faces);
        } else if (type == opengl_wrapper::obj_parser_t::line_type_t::texture_coordinate) {
            parser.get_line(texture_coords);
        } else if (type == opengl_wrapper::obj_parser_t::line_type_t::smoothing) {
            parser.get_line(smooth);
        } else {
            parser.get_line();
        }
    }

    EXPECT_EQ(name, "Cube");

    EXPECT_EQ(vertices.size(), 8);
    EXPECT_EQ(vertices[0], glm::vec3(1.0F, 1.0F, -1.0F));
    EXPECT_EQ(vertices[2], glm::vec3(1.0F, 1.0F, 1.0F));
    EXPECT_EQ(vertices[4], glm::vec3(-1.0F, 1.0F, -1.0F));
    EXPECT_EQ(vertices[7], glm::vec3(-1.0F, -1.0F, 1.0F));

    EXPECT_EQ(texture_coords.size(), 14);
    EXPECT_EQ(texture_coords[0], glm::vec2(0.625000F, 0.500000F));
    EXPECT_EQ(texture_coords[3], glm::vec2(0.625000F, 0.750000F));
    EXPECT_EQ(texture_coords[6], glm::vec2(0.375000F, 1.000000F));
    EXPECT_EQ(texture_coords[9], glm::vec2(0.625000F, 0.250000F));
    EXPECT_EQ(texture_coords[13], glm::vec2(0.125000F, 0.750000F));

    opengl_wrapper::face_t f00{1, 1, 1};
    opengl_wrapper::face_t f02{7, 3, 1};
    opengl_wrapper::face_t f10{4, 5, 0};
    opengl_wrapper::face_t f12{7, 6, 0};
    opengl_wrapper::face_t f20{8, 0, 3};
    opengl_wrapper::face_t f22{5, 0, 3};
    opengl_wrapper::face_t f30{6, 0, 0};
    opengl_wrapper::face_t f32{4, 0, 0};

    EXPECT_EQ(faces.size(), 6);
    EXPECT_EQ(faces[0].size(), 4);
    EXPECT_EQ(faces[0][0], f00);
    EXPECT_EQ(faces[0][2], f02);
    EXPECT_EQ(faces[1].size(), 4);
    EXPECT_EQ(faces[1][0], f10);
    EXPECT_EQ(faces[1][2], f12);
    EXPECT_EQ(faces[2].size(), 4);
    EXPECT_EQ(faces[2][0], f20);
    EXPECT_EQ(faces[2][2], f22);
    EXPECT_EQ(faces[3].size(), 4);
    EXPECT_EQ(faces[3][0], f30);
    EXPECT_EQ(faces[3][2], f32);
    EXPECT_EQ(smooth, false);
}