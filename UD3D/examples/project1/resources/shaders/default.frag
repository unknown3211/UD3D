#version 330 core

out vec4 fragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;

void main()
{
   vec3 Normal = normalize(normal);
   fragColor = texture(tex0, texCoord);
}