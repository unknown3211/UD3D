#include "misc/skybox.h"
#include "buffers/vao.h"
#include "buffers/vbo.h"
#include "buffers/ebo.h"
#include <stb_image.h>

/// <summary>
///  THIS SHIT NEEDS FIXING, SOME ARE FLIPPED CAUSING SKYBOX TO BE FUCKED ///
/// </summary>

void Skybox::Load(const std::string faces[6])
{
    stbi_set_flip_vertically_on_load(false);

    float s = 1.0f;

    vertices = {
        {{-s,-s, s}}, {{ s,-s, s}}, {{ s,-s,-s}}, {{-s,-s,-s}},
        {{-s, s, s}}, {{ s, s, s}}, {{ s, s,-s}}, {{-s, s,-s}}
    };

    indices = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,1,5, 5,4,0,
        3,2,6, 6,7,3,
        0,3,7, 7,4,0,
        1,2,6, 6,5,1
    };

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(vertices);
    ebo = std::make_unique<EBO>(indices);

    vao->Bind();
    vbo->Bind();
    ebo->Bind();

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    
    vao->Unbind();

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, c;
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &c, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
}

void Skybox::Draw(int width, int height, Shader& shader, Camera& camera)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    shader.Bind();

    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    shader.SetInt("skybox", 0);

    vao->Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao->Unbind();

    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}