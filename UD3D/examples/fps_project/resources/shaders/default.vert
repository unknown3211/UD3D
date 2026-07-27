#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec4 aBoneIds;
layout (location = 5) in vec4 aBoneWeights;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 crntPos;

uniform mat4 bone_transforms[100];
uniform int hasBones;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);
    
    if (hasBones == 1)
    {
        mat4 boneTransform = mat4(0.0);
        float totalWeight = 0.0;
        
        for (int i = 0; i < 4; i++)
        {
            int boneId = int(aBoneIds[i]);
            if (boneId >= 0 && boneId < 100)
            {
                boneTransform += bone_transforms[boneId] * aBoneWeights[i];
                totalWeight += aBoneWeights[i];
            }
        }
        
        if (totalWeight < 0.001)
        {
            boneTransform = mat4(1.0);
        }

        totalPosition = boneTransform * vec4(aPos, 1.0);
        totalNormal = mat3(boneTransform) * aNormal;
    }
    else
    {
        totalPosition = vec4(aPos, 1.0);
        totalNormal = aNormal;
    }

    vec4 worldPosition = model * totalPosition;
    crntPos = vec3(worldPosition);
    gl_Position = projection * view * worldPosition;
    
    color = aColor;
    texCoord = aTex;
    normal = normalize(mat3(transpose(inverse(model))) * totalNormal);
}
