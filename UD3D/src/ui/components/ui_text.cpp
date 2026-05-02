#include <glad/glad.h>
#include "ui/components/ui_text.h"
#include "logs.h"

void UIText::initText(std::string fontPath, int fontSize)
{
    if (FT_Init_FreeType(&ft))
        Log(error, "Could not init FreeType Library\n");

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        Log(error, "Failed to load font\n");

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Log(error, "Failed to load Glyph\n");
            continue;
        }

        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
            face->glyph->bitmap.width, 
            face->glyph->bitmap.rows, 
            0, 
            GL_RED, 
            GL_UNSIGNED_BYTE, 
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = { 
            tex,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
            (unsigned int)face->glyph->advance.x
        };

        characters.insert({ c, character });
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();

    vao->Bind();
    vbo->Init(sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
    vao->LinkAttrib(*vbo, 0, 4, GL_FLOAT, 4 * sizeof(float), (void*)0);

    vao->Unbind();
    vbo->Unbind();
}

void UIText::renderText(Shader& shader, std::string text, glm::vec2 position, float scale, glm::vec3 color, int s_width, int s_height)
{
    shader.Bind();

    glm::mat4 projection = glm::ortho(0.0f, (float)s_width, 0.0f, (float)s_height);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);
    shader.SetMat4("model", model);

    shader.SetVec3("textColor", color);
    shader.SetInt("text", 0);

    glActiveTexture(GL_TEXTURE0);

    vao->Bind();
    vbo->Bind();

    float cursorX = position.x;
    float cursorY = position.y;

    for (char c : text)
    {
        Character ch = characters[c];

        float xpos = cursorX + ch.Bearing.x * scale;
        float ypos = cursorY - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursorX += (ch.Advance >> 6) * scale;
    }

    vao->Unbind();
    vbo->Unbind();
}