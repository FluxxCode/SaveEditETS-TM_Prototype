#pragma once

#include "ETS2/Profile/Profile.h"
#include "Application/Config/Config.h"

class SkillTab
{
private:
	//Variables
	float stupd1ColumnSize = 0.0f;

	//Skills
	//ADR
	std::vector<bool> adr;
	std::vector<int> adrVal = { 1, 2, 4, 8, 16, 32 }; //Values of the adr skills

	//Distance
	std::vector<bool> distanceHovered;
	int selectedDistance = 0;

	//High-Value Cargo 
	std::vector<bool> heavyHovered;
	int selectedHeavy = 0;

	//Fragile Cargo
	std::vector<bool> fragileHovered;
	int selectedFragile = 0;

	//Urgent delivery
	std::vector<bool> urgentHovered;
	int selectedUrgent = 0;

	//Mechanical
	std::vector<bool> mechanicalHovered;
	int selectedMechanical = 0;

	Profile* profile = NULL;
	Config* config = NULL;
	ConsoleWindow* consoleWindow = NULL;
	Utility::ResourceManager* resourceManager = NULL;

	//Functions
	void LoadADR();

public:
	//Constructors/Destructors
	SkillTab();
	virtual ~SkillTab();

	//Functions
	void Update();

	void Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow);

	void Save();

	//Getter/Setter
};