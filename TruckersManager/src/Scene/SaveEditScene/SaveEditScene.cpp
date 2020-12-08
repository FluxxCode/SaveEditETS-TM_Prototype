#include "tmpch.h"

#include "SaveEditScene.h"
#include "Application/Application.h"
#include "Scene/GameScene/GameScene.h"
#include "Scene/ConfigScene/ConfigScene.h"
#include "Scene/MoreScene/MoreScene.h"-

//Functoins
void SaveEditScene::Init()
{
	this->InitWindows();
}

void SaveEditScene::InitWindows()
{
	this->loadProfileWindow.SetBtnCreditsCallback([this]() { this->showCredits = true; });
	this->loadProfileWindow.SetBtnLoadCallback([this]() { this->LPWBtnLoad(); });

	this->profileWindow.SetBtnChangeProfileCallback([this]() { this->PWBtnChangeProfile(); });
	this->profileWindow.SetBtnSaveCallback([this]() { this->PWBtnSave(); });
}

void SaveEditScene::UpdateMainSelection()
{
	ImGui::SetNextWindowPos(ImVec2(1280 / 2 - this->mainSelectionX / 2, 0));
	ImGui::Begin("mainselection", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	this->mainSelectionX = ImGui::GetWindowPos().x;

	ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected2);
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_game").c_str()))
		this->LoadScene(new GameScene(this->GetApp()));
	ImGui::PopStyleColor(1);

	ImGui::SameLine();

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_Button]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_Button]);
	ImGui::Button(this->GetConfig()->GetLabel("mt_save_edit").c_str());
	ImGui::PopStyleColor(2);

	ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected2);
	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_config").c_str()))
		this->LoadScene(new ConfigScene(this->GetApp()));

	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_more").c_str()))
		this->LoadScene(new MoreScene(this->GetApp()));
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::Text(("V" + this->GetApp()->GetAppVersion()).c_str());

	ImGui::End();
}

//void SaveEditScene::CheckVersion(bool* val)
//{
//	Utility::Timer timerGetVersion("[UpdaterAPP] Time used to check for updates: ");
//
//	Utility::AddLog("[UpdaterAPP] Downloading version...");
//	std::string currentVersion = this->GetConfig()->GetConfig("version");
//
//	//Download the version file and get the version of the current stable version
//	//https://drive.google.com/uc?export=download&id=ID	
//
//	std::string URL = "https://drive.google.com/uc?export=download&id=1qBvJz6JVkg3l8BstaYdfvU4-myEewEXt";
//	std::string savePath = "latest_version_temp.txt";
//
//	fs::remove(savePath);
//
//	//Count time
//	Utility::Timer timer = Utility::Timer("");
//	timer.Start();
//
//	//Get data from the URL and write it into a savePath
//	HRESULT result = URLDownloadToFileA(NULL, URL.c_str(), savePath.c_str(), 0, NULL);
//	if (result != S_OK) //Error when downloading the file
//	{
//		if (result == E_OUTOFMEMORY)
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The buffer length is invalid, or there is insufficient memory to complete the operation.");
//		else
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The specified resource or callback interface was invalid.");
//
//		if (val) *val = false;
//		return;
//	}
//	timer.Stop();
//
//	uintmax_t size = fs::file_size(savePath);
//	Utility::AddLog("[UpdaterAPP] Downloaded file: \"" + URL + "\".(" + std::to_string(size) + " Bytes) Time used: " + std::to_string(timer.GetTimeS()) + "s");
//
//	//Read data from the savePath file
//	std::fstream file(savePath);
//	if (file.is_open())
//	{
//		std::getline(file, this->latestVersion);
//		file.close();
//	}
//	else
//	{
//		Utility::AddLog("[UpdaterAPP] ERROR: Unable to open file: \"" + savePath + "\n");
//		if (val) *val = false;
//		return;
//	}
//
//	//Check if the file was loaded correctly
//	if (this->latestVersion.find(".") == std::string::npos || this->latestVersion.size() > 250)
//	{
//		Utility::AddLog("[UpdaterAPP] ERROR: Invalid version: " + this->latestVersion);
//		if (val) *val = false;
//		return;
//	}
//
//	Utility::AddLog("[UpdaterAPP] Current version: " + currentVersion + ", latest version: " + this->latestVersion);
//	//Check if a new version is available
//	if (currentVersion != this->latestVersion && this->GetConfig()->GetConfig("ignore_version") != this->latestVersion)
//	{
//		//New version available
//		this->GetPatchNotes();
//		fs::remove(savePath); //Delete the new file
//		if (val) *val = true;
//		return;
//	}
//
//	fs::remove(savePath); //Delete the new file
//
//	if (val) *val = false;
//	return;
//}
//
//void SaveEditScene::GetPatchNotes()
//{
//	Utility::AddLog("[UpdaterAPP] Downloading patchnotes...");
//
//	//Download the version file and get the version of the current stable version
//	//https://drive.google.com/uc?export=download&id=ID
//
//	//URL to the file
//	std::string URL = "https://drive.google.com/uc?export=download&id=1WYd6OdFn3CfEZWgZFr1b79XEyaIsuIHW";
//	std::wstring wstr = std::wstring(URL.begin(), URL.end());
//	LPCWSTR lpw = wstr.c_str();
//
//	//File in that the data from the url gets written
//	std::string savePath = "patchnotes.txt";
//	std::wstring wSavePath = std::wstring(savePath.begin(), savePath.end());
//	LPCWSTR lSavePath = wSavePath.c_str();
//
//	fs::remove(savePath);
//
//	//Count time
//	Utility::Timer timer = Utility::Timer("");
//
//	//Get data from the URL and write it into a savePath
//	HRESULT result = URLDownloadToFile(NULL, lpw, lSavePath, 0, NULL);
//	if (result != S_OK) //Error when downloading the file
//	{
//		if (result == E_OUTOFMEMORY)
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The buffer length is invalid, or there is insufficient memory to complete the operation.");
//		else
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The specified resource or callback interface was invalid.");
//
//		return;
//	}
//	timer.Stop();
//
//	uintmax_t size = fs::file_size(savePath);
//	Utility::AddLog("[UpdaterAPP] Downloaded file: \"" + URL + "\".(" + std::to_string(size) + " Bytes) Time used: " + std::to_string(timer.GetTimeS()) + "s");
//
//	//Read data from the savePath file
//	bool error = true;
//
//	std::string line;
//	std::fstream file(savePath);
//	if (file.is_open())
//	{
//		while (std::getline(file, line))
//		{
//			if (line.find("#patchnotes") != std::string::npos)
//			{
//				error = false;
//				continue;
//			}
//
//			this->versionPatchNotes += line + "\n";
//		}
//
//		file.close();
//	}
//	else
//	{
//		Utility::AddLog("[UpdaterAPP] ERROR: Unable to open file: \"" + savePath + "\"");
//		return;
//	}
//
//	if (error)	this->versionPatchNotes = "Error when loading version patchnotes!";
//
//	this->newVersion = true;
//}
//
//void SaveEditScene::UpdateUpdater()
//{
//	Utility::AddLog("[UpdaterAPP] Updating updater...");
//
//	//Download the version file and get the version of the current stable version
//	//https://drive.google.com/uc?export=download&id=ID
//
//	//URL to the file
//	std::string URL = "https://drive.google.com/uc?export=download&id=1P-fQXIWKHsiToZjaEVVOYsgCTlL3yiAw";
//	std::wstring wstr = std::wstring(URL.begin(), URL.end());
//	LPCWSTR lpw = wstr.c_str();
//
//	//File in that the data from the url gets written
//	std::string savePath = "upd_temp.exe";
//	std::wstring wSavePath = std::wstring(savePath.begin(), savePath.end());
//	LPCWSTR lSavePath = wSavePath.c_str();
//
//	fs::remove(savePath);
//
//	//Count time
//	Utility::Timer timer = Utility::Timer("");
//
//	//Get data from the URL and write it into a savePath
//	HRESULT result = URLDownloadToFile(NULL, lpw, lSavePath, 0, NULL);
//	if (result != S_OK) //Error when downloading the file
//	{
//		if (result == E_OUTOFMEMORY)
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The buffer length is invalid, or there is insufficient memory to complete the operation.");
//		else
//			Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The specified resource or callback interface was invalid.");
//
//		return;
//	}
//	timer.Stop();
//
//	uintmax_t size = fs::file_size(savePath);
//	Utility::AddLog("[UpdaterAPP] Downloaded file: \"" + URL + "\".(" + std::to_string(size) + " Bytes) Time used: " + std::to_string(timer.GetTimeS()) + "s");
//
//	//Remove upd.exe
//	Utility::AddLog("[UpdaterAPP] Remove: \"upd.exe\"");
//	fs::remove("upd.exe");
//
//	//Copy new upd version
//	Utility::AddLog("[UpdaterAPP] Copy: \"" + savePath + "\" to \"upd.exe\"");
//	fs::copy(savePath, "upd.exe");
//
//	//Remove upd temp
//	Utility::AddLog("[UpdaterAPP] Remove: \"" + savePath +"\"");
//	fs::remove(savePath);
//}
//
//void SaveEditScene::StartUpdater()
//{
//	Utility::AddLog("[UpdaterAPP] Updating application...");
//
//	//Close the app and update it
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//	ZeroMemory(&pi, sizeof(pi));
//
//	std::string str = "upd.exe";
//	std::wstring wstr = std::wstring(str.begin(), str.end());
//
//	LPCWSTR pName = wstr.c_str();
//
//	Utility::AddLog("[UpdaterAPP] Create process: \"" + str + "\"");
//	//Creat process
//	if (CreateProcess(pName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
//	{
//		//Close process and thread handles. 
//		CloseHandle(pi.hProcess);
//		CloseHandle(pi.hThread);
//
//		//Close Application
//		Utility::AddLog("[UpdaterAPP] Closing application...");
//		exit(2);
//	}
//	else
//	{
//		Utility::AddLog("[UpdaterAPP] ERROR: Unable to create process: \"" + str + "\": " + std::to_string(GetLastError()));
//	}
//}

void SaveEditScene::SaveProfile()
{
	//Save the window values
	this->profileWindow.Save();
	this->companyWindow.Save();
	this->centerWindow.Save();

	//Save the profile
	this->profile->SaveData(this->profileWindow.GetBackup());

	//Logging
	this->consoleWindow.PushMessage("Saved: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + ".");
	Utility::AddLog("[MainScene] Saved: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + ".");

	if (this->profile->changedName)	this->PWBtnChangeProfile();
}

void SaveEditScene::CreateBackup()
{
	//Create backup from profile.sii
	fs::remove(this->profile->GetPath() + "\\profile_save_edit_backup.sii");
	fs::copy(this->profile->GetPath() + "\\profile.sii", this->profile->GetPath() + "\\profile_save_edit_backup.sii");

	//Create backup from game.sii
	fs::remove(this->profile->GetSave()->GetPath() + "\\game_save_edit_backup.sii");
	fs::copy(this->profile->GetSave()->GetPath() + "\\game.sii", this->profile->GetSave()->GetPath() + "\\game_save_edit_backup.sii");

	//Save
	Utility::AddLog("[MainScene] Saving: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + "...");
	this->consoleWindow.PushMessage("Saving: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + "...");
	this->AddStartUpdateFunc([this]() { this->SaveProfile(); });
}

//Constructors/Destructors
SaveEditScene::SaveEditScene(Application* app)
	: Scene(app), profile(NULL)
{

}

SaveEditScene::~SaveEditScene()
{

}

//Functions
void SaveEditScene::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->UpdateMainSelection();

	for (unsigned int i = 0; i < this->startUpdateFunc.size(); i++)	
		this->startUpdateFunc[i]();
	this->startUpdateFunc.clear();

	if (this->newVersion)
	{
		this->UpdateNewVersionWindow();
		return; //Only draw new version window
	}

	if (this->showCredits && this->profile)			    this->creditsWindow.Udpate(this->GetConfig(), &this->showCredits);
	if (this->showPatchnotes && this->profile)			this->patchnotesWindow.Update(this->GetConfig(), &this->showPatchnotes);
	if (this->showConfigSharingWindow && this->profile) this->configSharingWindow.Update(this->GetConfig(), &this->consoleWindow, &this->showConfigSharingWindow, this->loadProfileWindow.GetProfiles());

	if (!this->profile)		this->UpdateLoadProfileWindow();
	if (this->profile)		this->UpdateProfileWindow();
	if (this->profile)		this->UpdateCompanyWindow();
	if (this->profile)		this->UpdateCenterWindow();


	//Main menu bar
	//if (this->profile)
	//{
	//	if (ImGui::BeginMainMenuBar())
	//	{
	//		if (ImGui::BeginMenu(this->GetConfig()->GetLabel("mmb_tools").c_str()))
	//		{
	//			ImGui::MenuItem(this->GetConfig()->GetLabel("mmb_config_sharing").c_str(), "", &this->showConfigSharingWindow);
	//			Utility::ImGuiToolTip(this->GetConfig()->GetLabel("mmb_config_sharing_tt").c_str());

	//			ImGui::EndMenu();
	//		}
	//		if (ImGui::BeginMenu(this->GetConfig()->GetLabel("mmb_more").c_str()))
	//		{
	//			ImGui::MenuItem(this->GetConfig()->GetLabel("mmb_credits").c_str(), "", &this->showCredits);
	//			ImGui::MenuItem(this->GetConfig()->GetLabel("mmb_patchnotes").c_str(), "", &this->showPatchnotes);
	//			ImGui::Separator();
	//			ImGui::MenuItem(this->GetConfig()->GetLabel("mmb_help").c_str(), "");

	//			ImGui::EndMenu();
	//		}
	//		ImGui::EndMainMenuBar();
	//	}
	//}
}

void SaveEditScene::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SaveEditScene::OnLoad()
{
	Utility::AddLog("[SaveEditScene] Loading scene: SaveEditScene...");
	this->Init();
	//if (Utility::StringToBool(this->GetConfig()->GetConfig("auto_update")))
	//{
	//	std::thread th([this] { this->CheckVersion(); });
	//	th.detach();
	//}
}

void SaveEditScene::OnEnd()
{
	Utility::AddLog("[SaveEditScene] Ending scene: SaveEditScene...");
}

//Windows
void SaveEditScene::UpdateLoadProfileWindow()
{
	this->loadProfileWindow.SetSize(ImVec2(this->GetApp()->GetDisplayW() - 150.0f, this->GetApp()->GetDisplayH() - 150.0f)); //Set the size because app is now defined
	this->loadProfileWindow.Update(this->GetApp()->GetAppVersion()); //Update the window
}

void SaveEditScene::UpdateProfileWindow()
{
	if (this->profileWindow.GetProfile() != this->profile)	this->profileWindow.SetProfile(this->profile); //Set the profile in the profile window
	this->profileWindow.Update(this->GetConfig());
}

void SaveEditScene::UpdateCompanyWindow()
{
	this->companyWindow.Update(this->GetConfig());
}

void SaveEditScene::UpdateCenterWindow()
{
	if (this->centerWindow.GetProfile() != this->profile)	this->centerWindow.SetProfile(this->profile); //Set the profile in the profile window
	this->centerWindow.Update();
}

void SaveEditScene::UpdateNewVersionWindow()
{
	ImGui::SetNextWindowSize(ImVec2(385.0f, 340.0f));
	ImGui::SetNextWindowPos(ImVec2(447.5f, 190.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowFocus();

	ImGui::Begin(this->GetConfig()->GetLabel("nvw_title").c_str(), NULL, ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);

	ImGui::TextColored(Utility::Colors::Green, this->GetConfig()->GetLabel("nvw_info").c_str());
	ImGui::Text((this->GetConfig()->GetLabel("nvw_current_version") + " " + this->GetApp()->GetAppVersion()).c_str());
	ImGui::Text((this->GetConfig()->GetLabel("nvw_latest_version") + " " + this->latestVersion).c_str());

	//Patchnotes
	ImGui::TextColored(Utility::Colors::Blue, this->GetConfig()->GetLabel("nvw_patch_notes").c_str());

	ImGui::BeginChild("##patchnoteschild", ImVec2(370.0f, 205.0f), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Text(this->versionPatchNotes.c_str());
	ImGui::EndChild();

	/* Buttons */
	if (ImGui::Button(this->GetConfig()->GetLabel("nvw_btn_update").c_str(), ImVec2(150.0f, 25.0f)))
	{
		Utility::AddLog("[UpdaterAPP] Updating...");

		this->newVersion = false;
		//this->UpdateUpdater();
		//this->StartUpdater();
	}
	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("nvm_btn_later").c_str(), ImVec2(100.0f, 25.0f)))
	{
		this->newVersion = false;
	}
	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("nvw_btn_ignore").c_str(), ImVec2(100.0f, 25.0f)))
	{
		this->GetApp()->GetConfig()->SetConfig("ignore_version", this->latestVersion);
		this->GetApp()->GetConfig()->Save();
		this->newVersion = false;
	}
	Utility::ImGuiToolTip(this->GetConfig()->GetLabel("nvw_btn_ignore_tt"));

	ImGui::End();
}

//Button functions
//LoadProfileWindow buttons
void SaveEditScene::LPWBtnLoad()
{
	this->profile = &this->loadProfileWindow.GetProfiles()->at(this->loadProfileWindow.GetSelectedProfile());
	if (this->profile)
	{
		this->consoleWindow.PushMessage("Loading: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr());

		this->profile->Load(); //Load the whole profile (whole save)

		this->companyWindow.SetProfile(this->profile);
		this->companyWindow.Load(&this->consoleWindow);

		this->profileWindow.SetProfile(this->profile);
		this->profileWindow.Load(&this->consoleWindow);

		this->centerWindow.SetProfile(this->profile);
		this->centerWindow.Load(this->profile, this->GetConfig(), this->GetResourceManager(), &this->consoleWindow);
	}
}

//ProfileWindow buttons
void SaveEditScene::PWBtnChangeProfile()
{
	this->profile = NULL;
	this->loadProfileWindow.UpdateProfiles();
}

void SaveEditScene::PWBtnSave()
{
	if (!this->profileWindow.GetBackup()) //Create no backup
	{
		//Logging
		Utility::AddLog("[MainScene] Saving: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + "...");
		this->consoleWindow.PushMessage("Saving: " + this->profile->GetAttribute("profile_name") + ", " + this->profile->GetSaveDisplayStr() + "...");

		this->AddStartUpdateFunc([this]() { this->SaveProfile(); });
	}
	else //Create backup
	{
		//Logging
		Utility::AddLog("[MainScene] Creating backup...");
		this->consoleWindow.PushMessage("Creating backup...");

		this->AddStartUpdateFunc([this]() { this->CreateBackup(); });
	}
}