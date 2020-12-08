#pragma once

#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"
#include "Application/Config/Config.h"
#include "ETS2/Profile/Profile.h"

class ConfigSharingWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	std::vector<Profile>* profiles;
	int selectedFromProfile = -1;
	int selectedToProfile = -1;

	bool config = true;
	bool configLocal = true;
	bool cotrols = true;

	float columnsWidth = 0;

	ConsoleWindow* consoleWindow = NULL;

	//Functions
	void BtnCopy(); //Function of the copy button

public:
	//Constructors/Destuctors
	ConfigSharingWindow(ImVec2 pos, ImVec2 size);
	virtual ~ConfigSharingWindow();
	
	//Functions
	void Update(Config* config, ConsoleWindow* consoleWindow, bool* open, std::vector<Profile>* profiles);
};