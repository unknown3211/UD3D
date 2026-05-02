#include "managers/config_manager.h"
#include "utils/utils.h"
#include "logs.h"

void LoadGameConfig(WindowDetails& details, std::string settingsPath)
{
	auto config = parseIni(settingsPath);
	if (!config.empty())
	{
		try {
			details.s_width = std::stoi(config["s_width"]);
			details.s_height = std::stoi(config["s_height"]);
			details.title = config["title"];
			details.vSync = config["vSync"] == "true";
			details.dev = config["dev"] == "true";
			details.editor = config["editor"] == "true";

			Log(info, "Game Config Loaded Successfully !\n");
		}
		catch (const std::exception& e) {
			Log(error, ("Error Parsing Values: " + std::string(e.what())).c_str());
		}
	}
	else
	{
		Log(error, ("Failed To Load Game Config INI File: " + settingsPath).c_str());
	}
}