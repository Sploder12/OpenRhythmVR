#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D scene;

void main()
{    
    fragColor = texture(scene, texCoords);
}