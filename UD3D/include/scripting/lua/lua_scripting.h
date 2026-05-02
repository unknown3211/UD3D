#pragma once

#include <vector>
#include <string>
#include <sol/sol.hpp>

struct LuaScript
{
    sol::environment env;
    sol::function onStart;
    sol::function onUpdate;
    sol::function onShutdown;
};

class LScripting
{
public:
    sol::state lua{};

    void Init();
    void LoadScriptsFromConfig(const std::string& configPath, const std::string& basePath);

    void CallStart();
    void CallUpdate(float dt);
    void CallShutdown();

private:
    std::vector<LuaScript> loadedScripts;

    void LoadScript(const std::string& filePath);
    void loadLuaFunctions();
};