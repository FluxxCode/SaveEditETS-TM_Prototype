#pragma once

#include "ETS2/Profile/Profile.h"
#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"

/*
	Window that shows stats about the profile
*/

class ProfielWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	Profile* profile;

	ImU64 xp = 0;
	bool backup = true;

	//Buttons
	std::function<void()> btnChangeProfileCallback;
	std::function<void()> btnSaveCallback;

public:
	//Constructors/Destcutors
	ProfielWindow(Profile* profile = NULL, ImVec2 pos = ImVec2(-5.0f, -5.0f), ImVec2 size = ImVec2(-5.0f, -5.0f));
	virtual ~ProfielWindow();

	//Functions
	void Update(Config* config);
	void Load(ConsoleWindow* consoleWindow);
	void Save(); //Set new values

	//Getter/Setter
	//Pos
	ImVec2 GetPos() { return this->pos; }
	void SetPos(ImVec2 pos) { this->pos = pos; }

	//Size
	ImVec2 GetSize() { return this->size; }
	void SetSize(ImVec2 size) { this->size = size; }

	//Profile
	Profile* GetProfile() { return this->profile; }
	void SetProfile(Profile* profile) { this->profile = profile; }

	bool GetBackup() { return this->backup; }

	//Buttons
	void SetBtnChangeProfileCallback(std::function<void()> func) { this->btnChangeProfileCallback = func; }
	void SetBtnSaveCallback(std::function<void()> func) { this->btnSaveCallback = func; }
};