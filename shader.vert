#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCord;
layout(location = 2) in float alabel;
out vec3 mycolor;
out vec2 TexCord;
out float label;
uniform mat4 self_matrix;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main()
{
    gl_Position = projection*view*model*self_matrix*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TexCord = aTexCord;
    label=alabel;
}
