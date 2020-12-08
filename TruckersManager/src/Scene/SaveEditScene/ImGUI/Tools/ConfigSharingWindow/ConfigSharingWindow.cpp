#include "tmpch.h"
#include "ConfigSharingWindow.h"

//Functions
void ConfigSharingWindow::BtnCopy()
{
	std::string fromPath = this->profiles->at(this->selectedFromProfile).GetPath();
	std::string toPath = this->profiles->at(this->selectedToProfile).GetPath();

	Utility::AddLog("[ConfigSharingWindow] Copy files from: \"" + fromPath + "\" to: \"" + toPath + "\"...");

	/* Delete and copy the selected files */
	if (this->config && fs::exists(fromPath + "\\config.cfg"))
	{
		Utility::AddLog("[ConfigSharingWindow] Remove: \"" + toPath + "\\config.cfg\"");
		fs::remove(toPath + "\\config.cfg");

		Utility::AddLog("[ConfigSharingWindow] Copy: \"" + fromPath + "\\config.cfg\" to: \"" + toPath + "\\config.cfg\"");
		fs::copy_file(fromPath + "\\config.cfg", toPath + "\\config.cfg");
	}
	if (this->configLocal && fs::exists(fromPath + "\\config_local.cfg"))
	{
		Utility::AddLog("[ConfigSharingWindow] Remove: \"" + toPath + "\\config_local.cfg\"");
		fs::remove(toPath + "\\config_local.cfg");

		Utility::AddLog("[ConfigSharingWindow] Copy: \"" + fromPath + "\\config_local.cfg\" to: \"" + toPath + "\\config_local.cfg\"");
		fs::copy_file(fromPath + "\\config_local.cfg", toPath + "\\config_local.cfg");
	}
	if (this->cotrols && fs::exists(fromPath + "\\controls.sii"))
	{
		Utility::AddLog("[ConfigSharingWindow] Remove: \"" + toPath + "\\controls.sii\"");
		fs::remove(toPath + "\\controls.sii");

		Utility::AddLog("[ConfigSharingWindow] Copy: \"" + fromPath + "\\controls.sii\" to: \"" + toPath + "\\controls.sii\"");
		fs::copy_file(fromPath + "\\controls.sii", toPath + "\\controls.sii");
	}

	consoleWindow->PushMessage("Copyed files!");

	this->selectedFromProfile = -1;
	this->selectedToProfile = -1;
}

//Constructors/Destuctors
ConfigSharingWindow::ConfigSharingWindow(ImVec2 pos, ImVec2 size)
	: pos(pos), size(size)
{

}

ConfigSharingWindow::~ConfigSharingWindow()
{

}

//Functions
void ConfigSharingWindow::Update(Config* config, ConsoleWindow* consoleWindow, bool* open, std::vector<Profile>* profiles)
{
	this->profiles = profiles;
	this->consoleWindow = consoleWindow;

	if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);
	this->columnsWidth = config->GetBiggestLabelSize("mmbupd1") + 15.0f;

	ImGui::SetNextWindowBgAlpha(1);

	ImGui::Begin(config->GetLabel("mmb_config_sharing").c_str(), open, ImGuiWindowFlags_NoResize);

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, this->columnsWidth);

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("config_sharing_from").c_str());
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(500.0f);

	bool selected = false;
	//From profile list box header
	if (ImGui::ListBoxHeader("##fromprofiles"))
	{
		for (unsigned int i = 0; i < profiles->size(); i++)
		{
			if (this->selectedFromProfile == i)	selected = true;
			else								selected = false;

			std::string label = profiles->at(i).GetName() + " - " + profiles->at(i).GetFolder();
			if (ImGui::Selectable(label.c_str(), &selected))
			{
				this->selectedFromProfile = i; //Set the selected profile
				Utility::AddLog("[ConfigSharingWindow] Selected from profile: " + std::to_string(this->selectedFromProfile));
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::NextColumn();
	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("config_sharing_to").c_str());
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(500.0f);

	selected = false;
	if (ImGui::ListBoxHeader("##toprofiles"))
	{
		for (unsigned int i = 0; i < profiles->size(); i++)
		{
			if (this->selectedToProfile == i)	selected = true;
			else								selected = false;

			std::string label = profiles->at(i).GetName() + " - " + profiles->at(i).GetFolder();
			if (ImGui::Selectable(label.c_str(), &selected))
			{
				this->selectedToProfile = i; //Set the selected profile
				Utility::AddLog("[ConfigSharingWindow] Selected to profile: " + std::to_string(this->selectedToProfile));
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::Columns(1);

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("config_sharing_to_copy").c_str());

	ImGui::Checkbox(config->GetLabel("config_sharing_checkbox_config").c_str(), &this->config);
	ImGui::Checkbox(config->GetLabel("config_sharing_checkbox_config_local").c_str(), &this->configLocal);
	ImGui::Checkbox(config->GetLabel("config_sharing_checkbox_cotrols").c_str(), &this->cotrols);

	ImGui::NewLine();

	if (this->selectedToProfile >= 0 && this->selectedFromProfile >= 0 && this->selectedFromProfile != this->selectedToProfile)
	{
		if (ImGui::Button(config->GetLabel("config_sharing_btn_copy").c_str(), ImVec2(585.0f, 25.0f)))	this->BtnCopy();
		Utility::ImGuiToolTip(config->GetLabel("config_sharing_btn_copy_tt").c_str());
	}
	else
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnDeactiveHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
		ImGui::Button(config->GetLabel("config_sharing_btn_copy").c_str(), ImVec2(585.0f, 25.0f));
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::SameLine();

		//Tooltips
		if (this->selectedToProfile < 0)								Utility::ImGuiToolTip(config->GetLabel("config_sharing_btn_copy_tt_err_1").c_str());
		else if (this->selectedFromProfile < 0)							Utility::ImGuiToolTip(config->GetLabel("config_sharing_btn_copy_tt_err_2").c_str());
		else if (this->selectedFromProfile == this->selectedToProfile)	Utility::ImGuiToolTip(config->GetLabel("config_sharing_btn_copy_tt_err_3").c_str());
	}

	ImGui::End();
}