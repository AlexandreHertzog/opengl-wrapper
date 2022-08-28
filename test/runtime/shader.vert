#version 330 core

layout (location = 0) in vec3 layout_pos;
layout (location = 1) in vec3 layout_color;

out vec3 vert_color;

void main()
{
    gl_Position = vec4(layout_pos, 1.0);
    vert_color = layout_color;
}