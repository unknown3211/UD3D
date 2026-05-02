#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>
#include "utils/glm_includes.h"

struct Shader
{
	GLuint id = 0;

	bool LoadShaderProgramFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);
	bool LoadShaderProgramFromData(const char* vertexShaderData, const char* fragmentShaderData);

	void Bind() const;
	void Clear();

	GLuint GetUniformLocation(const char* name);

	void SetMat4(const std::string& name, const glm::mat4& matrix);
	void SetVec4(const std::string& name, const glm::vec4& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetMat4Array(const std::string& name, const glm::mat4* matrices, std::size_t count);
};
