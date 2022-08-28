#version 330 core
layout (location = 0) in vec3 layout_pos;
layout (location = 1) in vec2 layout_tex_coord;
layout (location = 2) in vec3 layout_normals;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
    gl_Position = uniform_projection * uniform_view * uniform_model * vec4(layout_pos, 1.0);
}