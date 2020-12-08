#pragma once

#include "Application/Config/Config.h"
#include "ETS2/Profile/Profile.h"
#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"

/*
	Center window tab Profile. Tab to edit the profile
*/

class ProfileTab
{
private:
	//Variables
	Profile* profile;
	float firstColumnSize = 1.0f;

	//Values
	char* profileName = { 0 };
	char* companyName = { 0 };

	bool female = true;
	bool male = false;

	//Textures
	GLuint* femaleIcon = NULL;
	GLuint* maleIcon = NULL;

	//Objects
	Config* config = NULL;
	Utility::ResourceManager* resourceManager = NULL;
	ConsoleWindow* consoleWindow = NULL;

	//Sizes
	float winX = 338.0f,
		  winY = 540.0f,
		  picX = 63.0f,
		  picY = 63.0f;

	//Image selection
	std::string selectedProfileIcon = "";
	std::string selectedLogo = "";
	std::string selectedBrand = "";

	//Functoins
	void Init();

	void UpdateWindow(); //Update the ImGui window
	void UpdateValues(); //Update the values

	void ProfileImages(); //Update profile image selection
	void CompanyImages(); //Update company image selection
	void BrandImages(); //Update truck image selection

public:
	//Constructors/Destructors
	ProfileTab();
	virtual ~ProfileTab();

	//Functions
	void Udpate();
	void LoadProfile(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow); //All profile names
	void Save();
};