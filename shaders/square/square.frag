#version 330 core

uniform vec4 uColor;
uniform float uTime;

#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692


out vec4 FragColor;

void main()
{
    vec4 color = uColor;
    color.rb *= (sin(PI * uTime)/2.0+0.5);
    FragColor = color;
}
