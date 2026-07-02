#version 330 core
// for ratio
uniform mat4 uProjection;
uniform mat4 uModel;
//

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
}
