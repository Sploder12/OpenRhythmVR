#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    texCoords = texcoords.xy;
}