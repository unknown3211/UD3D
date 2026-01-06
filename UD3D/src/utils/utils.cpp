#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "utils/utils.h"

float DeltaTime()
{
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return deltaTime;
}

float Rotation()
{
	static float rotation = 0.0f;
	rotation += 50.0f * DeltaTime();
	if (rotation > 360.0f)
		rotation -= 360.0f;
	return rotation;
}

std::unordered_map<std::string, std::string> parseIni(const std::string& filename) {
    std::unordered_map<std::string, std::string> map;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        map[key] = value;
    }

    return map;
}