#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D scene;

#define PI 3.14159265358979323844

void main()
{
    const float r_inner=0.125; 
    const float r_outer=0.5; 

    vec2 x = texCoords - vec2(0.5);
    float radius = length(x);
    float angle = atan(x.y, x.x) + 0.5 * PI;

    vec2 tc_polar; // the new polar texcoords
    // map radius so that for r=r_inner -> 0 and r=r_outer -> 1
    tc_polar.s = ( radius - r_inner) / (r_outer - r_inner);

    // map angle from [-PI,PI] to [0,1]
    tc_polar.t = angle * 0.5 / PI + 0.5;
    if (tc_polar.s > r_outer * 2 || tc_polar.s < (r_inner * 0.5))
    {
        FragColor = vec4(0.0);
    }
    else
    {
        // texture mapping
        FragColor = texture2D(scene, tc_polar);
    }
} 