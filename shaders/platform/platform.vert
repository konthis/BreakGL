#version 330 core
// for ratio
uniform mat4 u_projection;
uniform mat4 u_model;
//

layout (location = 0) in vec3 a_pos;

out vec3 v_pos;

void main()
{
    v_pos = a_pos;
    gl_Position = u_projection * u_model * vec4(a_pos, 1.0);
}
