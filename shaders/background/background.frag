#version 330 core

uniform vec4 u_color;
uniform float u_time;

in vec3 v_pos;

#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692

out vec4 FragColor;

void main()
{
    float line = mod(gl_FragCoord.y, 4.0) < 2.0 ? 0.95 : 1.0; 
    FragColor = u_color*line;
}
