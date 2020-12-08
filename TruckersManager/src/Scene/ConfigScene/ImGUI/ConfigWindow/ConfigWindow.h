#pragma once

#include "Application/Config/Config.h"
#include "ETS2/Profile/Profile.h"
#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"
#include "Utility/ResourceManager/ResourceManager.h"

/*
	Config tab
*/

class ConfigWindow
{
private:
	//Variables
	ImVec2 pos  = ImVec2(35.0f, 35.0f);
	ImVec2 size = ImVec2(1210.0f, 650.0f);

	//System vars
	std::string language = "";
	int startTab = 0;
	bool console = false;
	//bool newVersionAvailable = true; //If a new version is available
	//bool autoUpdate = true; 

	//Style vars
	std::vector<std::string> styles; //All styles
	int selectedStyle = 0; //Currently selected style

	char newTemplateName[62] = { 0 };

	bool changedStyle = false;
	ImGuiStyle& style = ImGui::GetStyle();
	bool windowBorder = true;
	bool childBorder  = true;
	bool frameBorder  = false;
	bool tabBorder    = false;

	float ctupd1ColumnSize = 1.0f;
	float ctupd2ColumnSize = 1.0f;

	bool firstLoop = false;

	//Objects
	Config* config = NULL;
	Utility::ResourceManager* resourceManager = NULL;
	
	//Function
	void ProgramConfig();

	void LoadStyles();

	void UpdateValues();
	void ResetConfig();

	void SaveConfig();

	//Button Callbacks
	std::function<void(bool* val)> btnSearchForUpdatesCallback;

public:
	//Constructors/Destructors
	ConfigWindow();
	virtual ~ConfigWindow();

	//Functions
	void Update();
	void Load(Config* config, Utility::ResourceManager* resourceManager);
	void Save();

	//Getter/Setter
	void SetBtnSearchForUpdatesCallback(std::function<void(bool* val)> func) { this->btnSearchForUpdatesCallback = func; }
};