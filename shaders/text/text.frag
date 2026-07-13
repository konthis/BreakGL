#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 u_textColor;
uniform float u_time;

#define WINDOW_WIDTH 800.0

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    float x = (gl_FragCoord.x/WINDOW_WIDTH)/2.0 +0.5;
    float line = mod(gl_FragCoord.y, 4.0) < 2.0 ? 0.85 : 1.0; 
    float pulse = pow(1.0-abs(sin(x-u_time)),6.0)/5.0;

    color = sampled * vec4(u_textColor.rgb * line + pulse, u_textColor.a);

}  