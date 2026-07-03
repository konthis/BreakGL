#version 330 core

uniform vec4 uColor;
uniform float uTime;

in vec3 vPos;

#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692

out vec4 FragColor;

float rand(vec2 seed) {
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
    vec4 color = uColor;
    float dist = length(vPos.xy);
    color.r *= (sin(PI * uTime)/4.0+0.6);
    color.b *= (sin(PI * uTime * 0.5)/4.0+0.6);
    color.g *= (sin(PI * uTime * 1.5)/4.0+0.6);
    FragColor = color;
    // FragColor = (smoothstep(0.4,0.45,dist) * (1.0 - smoothstep(0.85,1.0,dist)))*color;
}
