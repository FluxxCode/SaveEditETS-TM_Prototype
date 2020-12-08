#pragma once

#include "Application/Config/Config.h"
#include "App/App.h"

class AppsWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	Config* config = nullptr;
	Utility::ResourceManager* resourceManager = nullptr;

	std::vector<App> apps = { };
	std::vector<std::string> appsConfig = { };

	DWORD	    ets2ProcessID = 0;
	std::string dllName = "tmapps.dll";
	std::string info = "";

	//Functions
	void LoadApps();
	void SaveApps();
	void Inject();

public:
	//Constructors/Destructors
	AppsWindow();
	virtual ~AppsWindow();

	//Fuctions
	void Update(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos, ImVec2 size);
};