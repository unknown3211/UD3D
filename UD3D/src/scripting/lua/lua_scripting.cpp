#include "scripting/lua/lua_scripting.h"
#include "scripting/lua/lua_functions.h"
#include "logs.h"
#include <fstream>
#include <nlohmann/json.hpp>

using s_json = nlohmann::json;

void LScripting::Init()
{
    lua.open_libraries(sol::lib::base);
    loadLuaFunctions();
}

void LScripting::LoadScriptsFromConfig(const std::string& configPath, const std::string& basePath)
{
    std::ifstream scriptsFile(configPath);

    if (!scriptsFile.is_open())
    {
        Log(error, ("Failed To Load Scripts Config File Path: " + configPath).c_str());
        return;
    }

    s_json config;
    scriptsFile >> config;

    for (auto& [fileName, shouldRun] : config.items())
    {
        if (shouldRun.get<bool>())
        {
            std::string fullPath = basePath + fileName;
            Log(info, ("Starting Script: " + fullPath).c_str());
            LoadScript(fullPath);
        }
    }
}

void LScripting::LoadScript(const std::string& filePath)
{
    LuaScript script;
    script.env = sol::environment(lua, sol::create, lua.globals());

    try
    {
        lua.script_file(filePath, script.env);

        script.onStart = script.env["OnStart"];
        script.onUpdate = script.env["OnUpdate"];
        script.onShutdown = script.env["OnShutdown"];

        loadedScripts.push_back(script);

        Log(info, ("Loaded Script: " + filePath).c_str());
    }
    catch (const std::exception& e)
    {
        Log(error, ("Failed To Load Script: " + filePath + " - " + e.what()).c_str());
    }
}

void LScripting::CallStart()
{
    for (auto& script : loadedScripts)
    {
        if (script.onStart.valid())
        {
            try
            {
                script.onStart();
            }
            catch (const std::exception& e)
            {
                Log(error, e.what());
            }
        }
    }
}

void LScripting::CallUpdate(float dt)
{
    for (auto& script : loadedScripts)
    {
        if (script.onUpdate.valid())
        {
            try
            {
                script.onUpdate(dt);
            }
            catch (const std::exception& e)
            {
                Log(error, e.what());
            }
        }
    }
}

void LScripting::CallShutdown()
{
    for (auto& script : loadedScripts)
    {
        if (script.onShutdown.valid())
        {
            try
            {
                script.onShutdown();
            }
            catch (const std::exception& e)
            {
                Log(error, e.what());
            }
        }
    }
}

void LScripting::loadLuaFunctions()
{
    initLuaFunctions();
}