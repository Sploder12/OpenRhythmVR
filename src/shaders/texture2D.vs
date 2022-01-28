#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 texCoords;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(vertex.x, vertex.y, 0.0, 1.0);
    texCoords = texcoords;
}