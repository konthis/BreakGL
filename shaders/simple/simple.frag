#version 330 core

uniform vec4 uColor;
uniform float uTime;

in vec3 vPos;

#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692

out vec4 FragColor;

void main()
{
    vec4 color = uColor;
    // float dist = length(vPos.xy)/20.0;
    // color.r *= (sin(PI * uTime)/4.0+0.5);
    // color.b *= (sin(PI * uTime * 0.5)/4.0+0.5);
    // color.g *= (sin(PI * uTime * 1.5)/4.0+0.5);
    FragColor = color;
}
