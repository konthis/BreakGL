/* comment out on real program */
// #ifdef GL_ES
// precision mediump float;
// #endif
//

#version 330 core
out vec4 FragColor;
in vec3 v_pos;



uniform vec4 u_color;
uniform vec2 u_resolution;
uniform float u_time;

#define PI      3.14159265358979323846
#define TWO_PI  6.28318530717958647692

// LATER MAKE IT A UNIFORM
#define SQUARE_SIDE 20.0

void main()
{
    vec2 st = v_pos.xy / (SQUARE_SIDE / 2.0); // normalized -1 to 1

    float line = mod(gl_FragCoord.y, 4.0) < 2.0 ? 0.95 : 1.0; // indie block style
    float frame = (1.0 - smoothstep(0.75,1.0,abs(st.x)))*(1.0-smoothstep(0.75,1.0,abs(st.y)));
    // gl_FragColor = u_color * line * frame;
    FragColor = u_color * line * frame;

}
