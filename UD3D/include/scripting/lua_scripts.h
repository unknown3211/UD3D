#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <lua.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

class LScripting
{
public:
	sol::state lua{};
	void LoadGameScripts(std::string configPath);
	void Init(std::string configPath);
	void RunLuaScript(std::string filePath);
	void LoadLuaFunctions();
};