#include "core/shapes.h"

static const char* vertexShader = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 0.0, 1.0);\n"
"   color = aColor;\n"
"}\0";

static const char* fragmentShader = "#version 330 core\n"
"in vec3 color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0f);\n"
"}\n\0";

void Shape::InitTriangle(float size, glm::vec3 color)
{
    vertices = {
        {{ 0.0f,  size, 0.0f}, {color.r, color.g, color.b}}, // top
        {{-size, -size, 0.0f}, {color.r, color.g, color.b}}, // bottom left
        {{ size, -size, 0.0f}, {color.r, color.g, color.b}}, // bottom right
    };

    indices = { 0, 1, 2 };

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(vertices);
    ebo = std::make_unique<EBO>(indices);

    vao->Bind();
    shader.LoadShaderProgramFromData(vertexShader, fragmentShader);

    GLsizei stride = sizeof(Vertex);
    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, stride, (void*)0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(float)));
}

void Shape::InitRectangle(float size, glm::vec3 color)
{
    vertices = {
      {{-size,  size, 0.0f}, {color.r, color.g, color.b}}, // top-left
      {{ size,  size, 0.0f}, {color.r, color.g, color.b}}, // top-right
      {{ size, -size, 0.0f}, {color.r, color.g, color.b}}, // bottom-right
      {{-size, -size, 0.0f}, {color.r, color.g, color.b}}, // bottom-left
    };

    indices = {
        0, 1, 2,
        0, 2, 3 
    };

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(vertices);
    ebo = std::make_unique<EBO>(indices);

    vao->Bind();
    shader.LoadShaderProgramFromData(vertexShader, fragmentShader);

    GLsizei stride = sizeof(Vertex);
    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, stride, (void*)0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(float)));
}

void Shape::Draw()
{
    shader.Bind();
    vao->Bind();
    ebo->Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    vao->Unbind();
}

void Shape::Delete()
{
    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();
}