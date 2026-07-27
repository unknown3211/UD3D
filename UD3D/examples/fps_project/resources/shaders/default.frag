#version 330 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 crntPos;
in vec4 bw;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;

void main()
{
    vec3 texColor = texture(tex0, texCoord).rgb;

    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor.rgb;

    vec3 N = normalize(normal);
    vec3 L = normalize(lightPos - crntPos);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor.rgb;

    vec3 result = (ambient + diffuse) * texColor;

    fragColor = vec4(result, 1.0);
}