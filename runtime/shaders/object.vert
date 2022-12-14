#version 330 core
layout (location = 0) in vec3 layout_pos;
layout (location = 1) in vec2 layout_tex_coord;
layout (location = 2) in vec3 layout_normals;

out vec2 vert_out_tex_coord;
out vec3 vert_out_normals;
out vec3 vert_out_position;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
    gl_Position = uniform_projection * uniform_view * uniform_model * vec4(layout_pos, 1.0);
    vert_out_tex_coord = layout_tex_coord;
    vert_out_normals = mat3(transpose(inverse(uniform_model))) * layout_normals;
    vert_out_position = vec3(uniform_model * vec4(layout_pos, 1.0));
}