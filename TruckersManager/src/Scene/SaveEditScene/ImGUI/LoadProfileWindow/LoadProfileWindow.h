#pragma once

#include "tmpch.h"
#include "ETS2/Profile/Profile.h"

namespace fs = std::filesystem;

class LoadProfileWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;
	float firstColumnSize = 0.0f;

	std::vector<std::string> paths; //Custom paths
	int selectedPath = -1;
	char newPath[320] = { 0 }; //New path

	//Profiles
	std::vector<Profile> profiles; //All profiles
	int selectedProfile = -1;

	//Saves
	std::vector<std::string> saves; //All saves that are in the current profile
	std::vector<std::string> saveDisplay; //Saves with string how they get displayed in ListBox
	int selectedSave = -1;

	std::string language = "";
	std::vector<std::string> errors;

	Config* config;
	Utility::ResourceManager* resourceManager;

	//Flaggs
	std::vector<GLuint*>flags;

	//Buttons
	std::function<void()> btnLoadCallback;
	std::function<void()> btnCreditsCallback;

	//Functions
	void LoadProfiles(); //Load the profiles in the path
	void LoadSaves(std::string path); //Load the saves

public:
	//Constructors/Destructors
	LoadProfileWindow(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos = ImVec2(-5.0f, -5.0f), ImVec2 size = ImVec2(-5.0f, -5.0f));
	virtual ~LoadProfileWindow();

	//Functions
	void Update(std::string version);
	void UpdateProfiles();

	//Getter/Setter
	ImVec2 GetPos() { return this->pos; }
	void SetPos(ImVec2 pos) { this->pos = pos; }

	ImVec2 GetSize() { return this->size; }
	void SetSize(ImVec2 size) { this->size = size; }

	std::vector<Profile>* GetProfiles() { return &this->profiles; }
	int GetSelectedProfile() { return this->selectedProfile; }

	//Button
	void SetBtnLoadCallback(std::function<void()> func) { this->btnLoadCallback = func; }
	void SetBtnCreditsCallback(std::function<void()> func) { this->btnCreditsCallback = func; }
};