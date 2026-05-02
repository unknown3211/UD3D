#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "udmath.h"
#include "globals/global_structs.h"

static float DeltaTime()
{
    static float lastTime = (float)glfwGetTime();
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}

static std::unordered_map<std::string, std::string> parseIni(const std::string& filename) {
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

static void Write_To_File(const char* fileName, const char* msg)
{
    std::ofstream logs;
    logs.open(fileName, std::ofstream::app | std::ofstream::out);
    logs << msg << "\n";
    logs.close();
}

static char* Read_Entire_File(const char* fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Error Opening File: " << fileName << "\n";
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

static const char* extract_name(const char* full)
{
    std::string p1 = full;
    static std::string p2;
    p2.clear();
    p2 = p1.substr(p1.find_last_of("/\\") + 1, (p1.length() - p1.find_first_of("/\\")) - (p1.length() - (p1.find_last_of(".")) + 1));
    return p2.c_str();
}

static std::string GetExtension(const std::string& path)
{
    size_t dot = path.find_last_of('.');
    if (dot == std::string::npos) return "";
    return path.substr(dot);
}

static std::vector<Vertex> CreateQuadVertices()
{
    return {
        {{-0.5f, -0.5f, 0.0f}, {1,1,1}, {0,0}},
        {{ 0.5f, -0.5f, 0.0f}, {1,1,1}, {1,0}},
        {{ 0.5f,  0.5f, 0.0f}, {1,1,1}, {1,1}},
        {{-0.5f,  0.5f, 0.0f}, {1,1,1}, {0,1}}
    };
}

static std::vector<GLuint> CreateQuadIndices()
{
    return { 0,1,2, 2,3,0 };
}