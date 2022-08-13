#version 330 core

out vec4 FragColor;

uniform vec4 runtime_color;

void main()
{
    FragColor = runtime_color;
}