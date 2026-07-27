#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * view * worldPos;

    TexCoords = vertex.zw;
}