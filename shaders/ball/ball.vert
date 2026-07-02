#version 330 core
// for ratio
uniform mat4 uProjection;
uniform mat4 uModel;
//

layout (location = 0) in vec3 aPos;

out vec3 vPos;

void main()
{
    vPos = aPos;
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
}
