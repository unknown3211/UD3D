#include "scripting/lua_scripts.h"
#include "scripting/lua_functions.h"

using s_json = nlohmann::json;

void LScripting::LoadGameScripts(std::string configPath)
{
    std::ifstream scriptsFile(configPath);

    if (!scriptsFile.is_open())
    {
        //Log(error, ("Failed To Load Scripts Config File Path: " + configPath).c_str());
        return;
    }

    try
    {
        s_json config;
        scriptsFile >> config;

        for (auto& [fileName, shouldRun] : config.items())
        {
            if (shouldRun.get<bool>())
            {
                //Log(info, ("Starting Script: " + std::string(fileName)).c_str());

                try
                {
                    RunLuaScript(fileName.c_str());
                    //Log(info, ("Started Script Successfully: " + std::string(fileName)).c_str());
                }
                catch (const std::exception& e)
                {
                    //Log(error, ("Failed To Start Script: " + std::string(fileName) + " - " + e.what()).c_str());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        //Log(error, ("Failed To Parse Scripts Json File: " + std::string(e.what())).c_str());
    }
}

void LScripting::Init(std::string configPath)
{
    lua.open_libraries(sol::lib::base);
    LoadLuaFunctions();
    LoadGameScripts(configPath);
}

void LScripting::RunLuaScript(std::string filePath)
{
    lua.script_file(filePath);
}

void LScripting::LoadLuaFunctions()
{
    InitLuaFunctions();
}