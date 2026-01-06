#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <unordered_map>
#include <string>

float DeltaTime();
float Rotation();
std::unordered_map<std::string, std::string> parseIni(const std::string& filename);