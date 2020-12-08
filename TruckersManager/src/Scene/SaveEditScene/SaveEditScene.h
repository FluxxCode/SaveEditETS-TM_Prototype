#pragma once

#include "Scene/Scene.h"
#include "Scene/SaveEditScene/ImGUI/Tools/CreditsWindow/CreditsWindow.h"
#include "Scene/SaveEditScene/ImGUI/Tools/ConfigSharingWindow/ConfigSharingWindow.h"
#include "Scene/SaveEditScene/ImGUI/LoadProfileWindow/LoadProfileWindow.h"
#include "Scene/SaveEditScene/ImGUI/ProfileWindow/ProfileWindow.h"
#include "Scene/SaveEditScene/ImGUI/CompanyWindow/CompanyWindow.h"
#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"
#include "Scene/SaveEditScene/ImGUI/CenterTabs/CenterWindow.h"
#include "Scene/SaveEditScene/ImGUI/Tools/PatchnotesWindow/PatchnotesWindow.h"

/*
	Scene to load the ETS profile
*/

namespace fs = std::filesystem;

class SaveEditScene : public Scene
{
private:
	//Variables
	Profile* profile;

	//Windows
	CreditsWindow creditsWindow             = CreditsWindow(ImVec2(440.0f, 160.0f), ImVec2(400.0f, 400.0f));
	ConfigSharingWindow configSharingWindow = ConfigSharingWindow(ImVec2(460.0f, 180.0f), ImVec2(600.0f, 435.0f));
	PatchnotesWindow patchnotesWindow	    = PatchnotesWindow(ImVec2(600.0f, 600.0f), ImVec2(600.0f, 600.0f));

	LoadProfileWindow loadProfileWindow = LoadProfileWindow(this->GetConfig(), this->GetResourceManager());
															  //        POS				        SIZE
	ProfielWindow profileWindow         = ProfielWindow(NULL, ImVec2(0.0f, 35.0f),     ImVec2(233.0f, 342.5f));
	CompanyWindow companyWindow			= CompanyWindow(NULL, ImVec2(0.0f, 377.5f),    ImVec2(233.0f, 342.5f));
	ConsoleWindow consoleWindow			= ConsoleWindow(	  ImVec2(233.0f, 624.0f),  ImVec2(1047.0f, 96.0f));
	CenterWindow  centerWindow          = CenterWindow(       ImVec2(233.0f, 35.0f),   ImVec2(1047.0f, 685.0f));

	bool showCredits = false;
	bool showPatchnotes = false;
	bool showConfigSharingWindow = false;

	int mainSelectionX = 0;

	std::vector<std::function<void()>> startUpdateFunc; //These funcoins will execute at the start of the Update function

	bool newVersion = false;
	std::string latestVersion = "";
	std::string versionPatchNotes = "";

	//Functions
	void Init();
	void InitWindows();

	void UpdateMainSelection();

	void AddStartUpdateFunc(std::function<void()> func) { this->startUpdateFunc.push_back(func); }

public:
	//Constructors/Destructors
	SaveEditScene(Application* app);
	virtual ~SaveEditScene();

	//Functions
	void Update();
	void Render();
	void OnLoad();
	void OnEnd();

	void SaveProfile(); //This function will call every save function of a profile
	void CreateBackup(); //Create backup of the current profile and save

	//void CheckVersion(bool* val = nullptr); //Check if a new version is available
	//void GetPatchNotes(); //Get the patchnotes from the last version

	//void UpdateUpdater(); //Update the updater
	//void StartUpdater(); //Update the app

	//Windows
	void UpdateLoadProfileWindow();
	void UpdateProfileWindow();
	void UpdateCompanyWindow();
	void UpdateCenterWindow();
	void UpdateNewVersionWindow(); //Window that shows the user that a new version is available

	//Button functions
	//LoadProfileWindow buttons
	void LPWBtnLoad(); //LoadProfileWindow button load

	//ProfileWindow buttons
	void PWBtnChangeProfile(); //ProfileWindow butotn changeProfile

	//CenterWindow buttons
	void PWBtnSave(); //CenterWindow button save

	//Getter/Setter
	void SetShowCredits(bool b) { this->showCredits = b; }
};