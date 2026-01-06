#include "core/shaders.h"

char* ReadEntireFile(const char* source)
{
    std::ifstream file;
    file.open(source, std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Error Opening File: " << source << "\n";
        return nullptr;
    }

    file.seekg(0, file.end);
    size_t size = file.tellg();
    file.seekg(0, file.beg);

    char* fileContent = new char[size + 1] {};
    file.read(fileContent, size);
    file.close();
    return fileContent;
}

bool Shader::LoadShaderProgramFromFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    char* vertexData = ReadEntireFile(vertexShaderPath);
    char* fragmentData = ReadEntireFile(fragmentShaderPath);

    if (!vertexData || !fragmentData)
    {
        if (vertexData) delete[] vertexData;
        if (fragmentData) delete[] fragmentData;
        return false;
    }

    bool result = LoadShaderProgramFromData(vertexData, fragmentData);

    delete[] vertexData;
    delete[] fragmentData;
    return result;
}

GLuint CreateShaderFromData(const char* data, GLenum shaderType)
{
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &data, nullptr);
    glCompileShader(shaderID);

    GLint result = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        GLint l = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &l);

        if (l)
        {
            char* message = new char[l];
            glGetShaderInfoLog(shaderID, l, &l, message);
            message[l - 1] = 0;
            std::cout << data << ":\n" << message << "\n";
            delete[] message;
        }
        else
        {
            std::cout << data << ":\nUnknown Error\n";
        }

        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}

bool Shader::LoadShaderProgramFromData(const char* vertexShaderData, const char* fragmentShaderData)
{
    auto vertexID = CreateShaderFromData(vertexShaderData, GL_VERTEX_SHADER);
    if (vertexID == 0) return false;

    auto fragmentID = CreateShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);
    if (fragmentID == 0)
    {
        glDeleteShader(vertexID);
        return false;
    }

    id = glCreateProgram();
    glAttachShader(id, vertexID);
    glAttachShader(id, fragmentID);
    glLinkProgram(id);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    GLint info = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &info);
    if (info != GL_TRUE)
    {
        GLint l = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);
        char* message = new char[l];
        glGetProgramInfoLog(id, l, &l, message);
        message[l - 1] = 0;
        std::cout << "Link Error: " << message << "\n";
        delete[] message;

        glDeleteProgram(id);
        id = 0;
        return false;
    }

    glValidateProgram(id);
    return true;
}

void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Clear()
{
    glDeleteProgram(id);
    *this = {};
}

GLuint Shader::GetUniformLocation(const char* name)
{
    GLuint result = glGetUniformLocation(id, name);

    if (result == -1)
    {
        std::cout << "Uniform Error: " << name << "\n";
    }

    return result;
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value)
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value)
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}