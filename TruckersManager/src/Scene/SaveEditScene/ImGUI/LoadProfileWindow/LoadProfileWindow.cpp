#include "tmpch.h"
#include "LoadProfileWindow.h"

#include "ETS2/SiiFileManager/SiiFileManager.h"

//Functions
void LoadProfileWindow::LoadProfiles()
{
	this->profiles.clear();

	for (unsigned int i = 0; i < this->paths.size(); i++)
	{
		if (fs::exists(this->paths[i]))
		{
			for (const auto& entry : fs::directory_iterator(this->paths[i])) //Get all profile folders
			{
				if (fs::exists(entry.path()) && fs::is_directory(entry.path()) && fs::exists(entry.path().string() + "\\profile.sii"))
				{
					Profile p = Profile(this->config, this->resourceManager, entry.path().string()); //Load the profile
					this->profiles.push_back(p); //Set the profile
				}
			}
		}
		else	this->errors.push_back("Path: \"" + this->paths[i] + "\" does not exists!");
	}
}

void LoadProfileWindow::LoadSaves(std::string path)
{
	this->saves.clear();
	this->saveDisplay.clear();
	if (fs::exists(path))
	{
		for (const auto& entry : fs::directory_iterator(path)) //Get all profile folders
		{
			if (fs::exists(entry.path()) && fs::is_directory(entry.path()))
			{
				fs::path savePath = entry.path().string();
				fs::path infoPath = entry.path().string() +"\\info.sii";

				this->saves.push_back(savePath.filename().string()); //Push folder to saves
				if (fs::exists(infoPath)) //Check if info exists to get the save name
				{
					std::string displayStr;
					if (savePath.filename().string().find("autosave") == std::string::npos)
					{
						SiiFileManager::Decrypt(infoPath.string());
						displayStr = SiiFileManager::LoadAttribut(infoPath.string(), "name");
					}
					else
						displayStr = "\"\"";

					if (displayStr == "\"\"")
					{
						this->saveDisplay.push_back(savePath.filename().string()); //Save has no name, set the display string to folder name
						continue;
					}
					if (displayStr[0] == '\"' && displayStr[displayStr.size() - 1] == '\"')
					{
						displayStr.erase(0, 1);
						displayStr.erase(displayStr.size() - 1);
					}
					this->saveDisplay.push_back(displayStr + " - " + savePath.filename().string()); //Set the display string to folder + name
				}
			}
		}
	}
	else	this->errors.push_back("Path: \"" + path + "\" does not exists!");
}

//Constructors/Destructors
LoadProfileWindow::LoadProfileWindow(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos, ImVec2 size)
	: config(config), resourceManager(resourceManager), pos(pos), size(size)
{
	//Ets porfiles path
	this->paths = this->config->GetArrayConfig("paths");
	if (paths.size() > 0)
	{
		if (paths[0] == "see_default")
			paths[0] = "C:\\Users\\" + Utility::GetUsername() + "\\Documents\\Euro Truck Simulator 2\\profiles"; //Set default path
		this->config->SetArrayConfig("paths", this->paths);
		this->config->Save();
	}
	
	strcpy(this->newPath, this->config->GetLabel("lpw_new_path").c_str());
	this->language = config->GetConfig("language");

	this->LoadProfiles(); //Load the profiles
}

LoadProfileWindow::~LoadProfileWindow()
{
	this->profiles.clear();
}

//Functions
void LoadProfileWindow::Update(std::string version)
{
	//Set the window pos & size
	if (this->pos.x != -5.0f && this->pos.y != -5.0f)	ImGui::SetNextWindowPos(this->pos);
	if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);

	if (this->firstColumnSize == 0.0f) 	this->firstColumnSize = this->config->GetBiggestLabelSize("lpwupd1") + 15.0f;

	bool selected = false;

	ImGui::Begin("Load profile", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); //Begin the window

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, this->firstColumnSize);

	//First line

	//Paths List
	ImGui::Text(this->config->GetLabel("lpw_paths").c_str());
	ImGui::NextColumn();
	if (ImGui::ListBoxHeader("##paths"))
	{
		for (unsigned int i = 0; i < this->paths.size(); i++)
		{
			selected = false;
			if (this->selectedPath == i) selected = true;

			ImGui::Text((std::to_string(i + 1) + ":").c_str());
			ImGui::SameLine();
			if (ImGui::Selectable(this->paths[i].c_str(), &selected))
				this->selectedPath = i;

			//Path does not exists
			if (!fs::exists(this->paths[i]))
			{
				Utility::ImGuiToolTip(this->config->GetLabel("lpw_path_err_tt"));
				ImGui::SameLine();
				ImGui::TextDisabled("(!)");
			}
		}
		ImGui::ListBoxFooter();
	}
	ImGui::SameLine();
	//Delete button
	if (this->selectedPath < 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnDeactiveHovered);
		ImGui::Button(this->config->GetLabel("lpw_btn_remove").c_str());
		Utility::ImGuiToolTip(this->config->GetLabel("lpw_btn_remove_tt").c_str());
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button(this->config->GetLabel("lpw_btn_remove").c_str()))
		{
			Utility::AddLog("[LoadProfileWindow] Button: Remove - Path: \"" + this->paths[this->selectedPath] + "\"");
			this->paths.erase(this->paths.begin() + this->selectedPath);
			this->UpdateProfiles();
			this->selectedPath = -1;
			this->config->SetArrayConfig("paths", this->paths);
			this->config->Save();
		}
	}

	ImGui::NextColumn();
	ImGui::NextColumn();

	//Add path
	ImGui::InputText("##inputtextpath", this->newPath, IM_ARRAYSIZE(this->newPath));
	ImGui::SameLine();
	if (ImGui::Button(this->config->GetLabel("lpw_btn_add").c_str()))
	{
		Utility::AddLog("[LoadProfileWindow] Button: Add - Path: \"" + std::string(newPath) + "\"");
		this->paths.push_back(this->newPath);
		this->UpdateProfiles();
		strcpy(this->newPath, this->config->GetLabel("lpw_new_path").c_str());
		this->config->SetArrayConfig("paths", this->paths);
		this->config->Save();
	}

	ImGui::NextColumn();

	//Second line
	ImGui::Text(this->config->GetLabel("lpw_profile").c_str());
	ImGui::NextColumn();

	//Profile drop down
	if (ImGui::ListBoxHeader("##profilesfgd"))
	{
		for (unsigned int i = 0; i < this->profiles.size(); i++)
		{
			if (this->selectedProfile == i)	selected = true;
			else							selected = false;

			//Get path id
			int pathID = 0;
			for (unsigned int x = 0; x < this->paths.size(); x++)
			{
				if (this->paths[x] == fs::path(this->profiles[i].GetPath()).parent_path().string())
				{
					pathID = x;
					break;
				}
			}

			std::string label = this->config->GetLabel("lpw_path") + " " + std::to_string(pathID + 1) + ": " + this->profiles[i].GetName() + " - " + this->profiles[i].GetFolder();
			if (ImGui::Selectable(label.c_str(), &selected))
			{
				this->selectedProfile = i; //Set the selected profile
				Utility::AddLog("[LoadProfileWindow] Selected profile: " + this->selectedProfile);
				this->LoadSaves(this->profiles[this->selectedProfile].GetPath() + "\\save"); //Load the saves of the profile
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::NextColumn();
	ImGui::Text(this->config->GetLabel("lpw_save").c_str());
	ImGui::NextColumn();

	//Save drop down
	if (ImGui::ListBoxHeader("##save"))
	{
		for (unsigned int i = 0; i < this->saveDisplay.size(); i++)
		{
			if (this->selectedSave == i)	selected = true;
			else							selected = false;

			if (ImGui::Selectable(this->saveDisplay[i].c_str(), &selected))		
				this->selectedSave = i; //Set the selected profile
		}
		ImGui::ListBoxFooter();
	}

	ImGui::SameLine();

	//Load button
	if (this->selectedProfile < 0 || this->selectedSave < 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnDeactiveHovered);
		ImGui::Button(this->config->GetLabel("lpw_btn_load").c_str());
		Utility::ImGuiToolTip(this->config->GetLabel("lpw_btn_load_err_tt").c_str());
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button(this->config->GetLabel("lpw_btn_load").c_str()))
		{
			this->errors.clear();
			this->profiles[this->selectedProfile].SetSaveFolderName(this->saves[this->selectedSave]); //Set the saveFolderName in the profile
			this->profiles[this->selectedProfile].SetSaveDisplayStr(this->saveDisplay[this->selectedSave]); //Set the display name of the save
			Utility::AddLog("[LoadProfileWindow] Button: Load - " + this->profiles[this->selectedProfile].GetFolder() + ", " + this->profiles[this->selectedProfile].GetSaveFolderName());
			this->btnLoadCallback();
		}
		Utility::ImGuiToolTip(this->config->GetLabel("lpw_btn_load_tt"));
	}
	ImGui::Columns(1);

	ImGui::End(); //End window
}

void LoadProfileWindow::UpdateProfiles()
{
	this->language = this->config->GetConfig("language");
	this->LoadProfiles();
}