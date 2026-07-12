/* comment out on real program */
// #ifdef GL_ES
// precision mediump float;
// #endif
//

#version 330 core
in vec3 v_pos;

uniform vec4 u_color;
uniform float u_time;
uniform vec2 u_resolution;
uniform float u_platformBigSide;


#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692

#define PLATFORM_SMALL_SIDE 10.0

out vec4 FragColor;

void main()
{
    vec2 st = vec2(v_pos.x / (u_platformBigSide/ 2.0), v_pos.y / (PLATFORM_SMALL_SIDE / 2.0)); // normalized -1 to 1
    float t = sin(u_time/2.0) * 0.5 + 0.5;
    float pulse = pow(1.0-abs(sin(st.x-u_time)),6.0)/5.0;
    vec4 u_color = mix(vec4(0.65, 0.0, 1.0, 1.0), vec4(0.0, 0.45, 0.80, 1.0), t);

    float frame = (1.0 - smoothstep(0.95,1.0,abs(st.x)))*(1.0-smoothstep(0.75,1.0,abs(st.y)));

    float line = mod(gl_FragCoord.y, 4.0) < 2.0 ? 0.95 : 1.0; 
    FragColor = (u_color*line+pulse)*frame;
}
