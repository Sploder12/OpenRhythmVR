#version 330 core
out vec4 fragColor;

void main()
{    
    fragColor = vec4(0.0, gl_FragCoord.x / 1280, gl_FragCoord.y / 720, 1.0);
}