#pragma once

#include "ETS2/Profile/Profile.h"
#include "ProfileTab/ProfileTab.h"
#include "SkillTab/SkillTab.h"
#include "VehicleTab/VehicleTab.h"

/*
	Calss for the center window
*/

class CenterWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	Profile* profile = NULL;
	Config* config = NULL;

	bool configOpen = false;

	//Tabs
	ProfileTab profileTab;
	SkillTab skillTab;
	VehicleTab vehicleTab;

public:
	//Constructors/Destructors
	CenterWindow(ImVec2 pos, ImVec2 size);
	virtual ~CenterWindow();

	//Functions
	void Update();
	void Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow);
	void Save();

	//Getter/Setter
	//Size
	ImVec2 GetSize() { return this->size; }
	void SetSize(ImVec2 size) { this->size = size; }

	//Profile
	Profile* GetProfile() { return this->profile; }
	void SetProfile(Profile* profile) { this->profile = profile; }
};