#include "tmpch.h"
#include "ConfigWindow.h"

//Functions
void ConfigWindow::ProgramConfig()
{
	ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("ct_system").c_str());
	ImGui::Separator();

	/* Languages */
	ImGui::Columns(2, "0", false);
	ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);

	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_language").c_str());
	ImGui::NextColumn();

	const char* items[] = { "en-US", "de-DE" };
	static const char* item_current = this->language.c_str();

	if (ImGui::BeginCombo("##languages", item_current))
	{
		for (int i = 0; i < IM_ARRAYSIZE(items); i++)
		{
			GLuint img = *this->resourceManager->LoadTexture("Resources\\Icons\\Freepik\\Flags\\" + (std::string)items[i] + ".png");
			if (img)	ImGui::Image((void*)(intptr_t)img, ImVec2(12.0f, 12.0f)); //Render the drive image
			ImGui::SameLine();

			bool is_selected = (item_current == items[i]);
			if (ImGui::Selectable(items[i], is_selected))
			{
				item_current = items[i];
				this->config->UpdateLanguage(item_current, false);
				this->language = item_current;
				this->Save();
				this->ctupd1ColumnSize = this->config->GetBiggestLabelSize("ctupd1") + 15.0f; //Set the column size of ctupd1
				this->ctupd2ColumnSize = this->config->GetBiggestLabelSize("ctupd2") + 15.0f; //Set the column size of ctupd2
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	/* Start tab */
	ImGui::NextColumn();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_start_tab").c_str());
	ImGui::NextColumn();
	std::vector<std::string> tabs = { this->config->GetLabel("mt_game"), this->config->GetLabel("mt_save_edit"), this->config->GetLabel("mt_config"), this->config->GetLabel("mt_more") };
	if (ImGui::BeginCombo("##starttab", tabs[this->startTab].c_str()))
	{
		for (int i = 0; i < tabs.size(); i++)
		{
			bool is_selected = (this->startTab == i);
			if (ImGui::Selectable(tabs[i].c_str(), is_selected))
			{
				this->startTab = i;
				this->Save();
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	/* Console */
	ImGui::NextColumn();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_console").c_str());
	ImGui::NextColumn();
	if (ImGui::Checkbox("##configcheckbox", &this->console))
	{
		if (console)
		{
			HWND console = GetConsoleWindow();
			ShowWindow(console, 1);
			HWND window = FindWindowA(NULL, "TruckersManager");
			SetFocus(window);
		}
		else
		{
			HWND console = GetConsoleWindow();
			ShowWindow(console, 0);
		}
		this->config->Save();
	}

	/* Udpates */
	//ImGui::NextColumn();
	//ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_auto_update").c_str());
	//Utility::ImGuiToolTip(this->config->GetLabel("ct_auto_update_tt"));
	//ImGui::NextColumn();
	//ImGui::Checkbox("##autoupdate", &this->autoUpdate);
	//Utility::ImGuiToolTip(this->config->GetLabel("ct_auto_update_tt"));
	//ImGui::NextColumn();
	//ImGui::NextColumn();
	//if (ImGui::Button(this->config->GetLabel("ct_btn_update").c_str()))
	//	this->btnSearchForUpdatesCallback(&this->newVersionAvailable);
	//if (!this->newVersionAvailable)
	//{
	//	ImGui::SameLine();
	//	ImGui::Text(this->config->GetLabel("ct_latest_version").c_str());
	//}
	ImGui::NextColumn();
	ImGui::NextColumn();
	//ImGui::TextDisabled(this->config->GetLabel("ct_save_time").c_str());
	ImGui::Columns(1);

	/* ImGui Style */
	ImGui::NewLine();
	ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("ct_style").c_str());
	ImGui::Separator();
	
	ImGui::Columns(2, "0", false);
	ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);

	//Templates
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_template").c_str());
	ImGui::NextColumn();

	bool selected = false;
	if (ImGui::ListBoxHeader("##styles24543"))
	{
		for (unsigned int i = 0; i < this->styles.size(); i++)
		{
			if (this->selectedStyle == i)	selected = true;
			else							selected = false;

			if (ImGui::Selectable(this->styles[i].c_str(), &selected))
			{
				this->selectedStyle = i; //Set the selected style
				this->config->LoadStyle(this->styles[i]);
				this->config->SetConfig("style", this->styles[i]);
				this->config->Save();
				strcpy(this->newTemplateName, this->styles[i].c_str());
				this->UpdateValues();
			}
		}
		ImGui::ListBoxFooter();
	}
	ImGui::NextColumn();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_save_as").c_str());
	ImGui::NextColumn();
	ImGui::InputText("##inputtexttemplates", this->newTemplateName, IM_ARRAYSIZE(this->newTemplateName));
	std::string str(this->newTemplateName);
	if (str != "" && str != " " && str != "Dark" && str != "Classic" && str != "Light" && str != "Default")
	{

		if (ImGui::Button((this->config->GetLabel("ct_btn_template_save") + this->newTemplateName).c_str(), ImVec2(470.0f, 20.0f)))
		{
			this->config->SaveStyle(str);
			this->config->LoadStyle(str);
			this->UpdateValues();
			this->LoadStyles();
		}
		ImGui::SameLine();
	}
	else
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnDeactiveHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
		ImGui::Button((this->config->GetLabel("ct_btn_template_save") + this->newTemplateName).c_str(), ImVec2(470.0f, 20.0f));
		Utility::ImGuiToolTip(this->config->GetLabel("ct_btn_template_save_tt"));
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::SameLine();
	}

	if (str != "" && str != " " && str != "Dark" && str != "Classic" && str != "Light" && str != "Default" && fs::exists("Styles\\" + str + ".ini"))
	{
		if (ImGui::Button(this->config->GetLabel("ct_btn_template_delete").c_str(), ImVec2(165.0f, 20.0f)))
		{
			fs::remove("Styles\\" + str + ".ini");
			this->LoadStyles();
			this->selectedStyle = 0;
		}
	}
	else
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnDeactiveHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
		ImGui::Button(this->config->GetLabel("ct_btn_template_delete").c_str(), ImVec2(165.0f, 20.0f));
		Utility::ImGuiToolTip(this->config->GetLabel("ct_btn_template_save_tt"));
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("ct_sizes").c_str());

	ImGui::Columns(2, "0", false);
	ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);

	//Main
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_scrollbar_size").c_str());		ImGui::NextColumn(); ImGui::SliderFloat("##ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_grab_min_size").c_str());      ImGui::NextColumn(); ImGui::SliderFloat("##GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
	//Borders
	ImGui::NextColumn(); ImGui::Columns(1);
	ImGui::Text(this->config->GetLabel("ct_borders").c_str()); 
	ImGui::Columns(2, "0", false);  ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);
						 ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_window_border_size").c_str()); ImGui::NextColumn(); ImGui::Checkbox("##WindowBorderSize", &this->windowBorder); this->style.WindowBorderSize = this->windowBorder;
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_child_border_size").c_str());  ImGui::NextColumn(); ImGui::Checkbox("##ChildBorderSize",  &this->childBorder);  this->style.ChildBorderSize  = this->childBorder;
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_frame_border_size").c_str());  ImGui::NextColumn(); ImGui::Checkbox("##FrameBorderSize",  &this->frameBorder);  this->style.FrameBorderSize  = this->frameBorder;
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_tab_border_size").c_str());    ImGui::NextColumn(); ImGui::Checkbox("##TabBorderSize",    &this->tabBorder);    this->style.TabBorderSize    = this->tabBorder;
	//Set the borders in the config
	this->config->SetStyleVal("imgui_window_border_size", Utility::BoolToString(this->windowBorder));
	this->config->SetStyleVal("imgui_child_border_size", Utility::BoolToString(this->childBorder));
	this->config->SetStyleVal("imgui_frame_border_size", Utility::BoolToString(this->frameBorder));
	this->config->SetStyleVal("imgui_tab_border_size", Utility::BoolToString(this->tabBorder));

	//RoundingI
	ImGui::NextColumn(); ImGui::Columns(1);
	ImGui::NextColumn(); ImGui::Text(this->config->GetLabel("ct_rounding").c_str()); ImGui::NextColumn();
	ImGui::Columns(2, "0", false);  ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);
						 ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_window_rounding").c_str());    ImGui::NextColumn(); ImGui::SliderFloat("##WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_child_rounding").c_str());     ImGui::NextColumn(); ImGui::SliderFloat("##ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_frame_rounding").c_str());     ImGui::NextColumn(); ImGui::SliderFloat("##FrameRounding", &style.FrameRounding, 0.0f, 9.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_scrollbar_rounding").c_str()); ImGui::NextColumn(); ImGui::SliderFloat("##ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 6.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_grab_rounding").c_str());      ImGui::NextColumn(); ImGui::SliderFloat("##GrabRounding", &style.GrabRounding, 0.0f, 7.0f, "%.0f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_tab_rounding").c_str());       ImGui::NextColumn(); ImGui::SliderFloat("##TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
	//Alignment
	ImGui::NextColumn(); ImGui::Columns(1);
	ImGui::NextColumn(); ImGui::Text(this->config->GetLabel("ct_alignment").c_str()); ImGui::NextColumn();
	ImGui::Columns(2, "0", false);  ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_window_title_align").c_str()); ImGui::NextColumn(); ImGui::SliderFloat2("##WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
	ImGui::NextColumn(); ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ct_button_text_align").c_str());  ImGui::NextColumn(); ImGui::SliderFloat2("##ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");

	ImGui::Columns(1);
	ImGui::NewLine();

	/* Colors */
	ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("ct_colors").c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, this->ctupd1ColumnSize);

	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		const char* name = ImGui::GetStyleColorName(i);
		std::string convertedName = "ct_imgui_";
		convertedName += Utility::GetConvertedIMGuiStyleColorName(i);

		ImGui::PushID(i);
		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel(convertedName).c_str());
		ImGui::NextColumn();
		if (ImGui::ColorEdit4("##color", (float*)&this->style.Colors[i], ImGuiColorEditFlags_AlphaBar))
		{
			this->changedStyle = true;
			this->selectedStyle = this->styles.size() - 1;
		}
		ImGui::PopID();
		ImGui::NextColumn();
	}

	this->UpdateValues();
}

void ConfigWindow::LoadStyles()
{
	this->styles.clear();
	if (!fs::exists("Styles"))
	{
		Utility::AddLog("[ConfigWindow] ERROR: Folder \"Styles\" does not exists!");
		return;
	}
	for (auto& p : fs::directory_iterator("Styles"))
	{
		if (!p.is_directory())
		{
			if (p.path().filename().extension() == ".ini")
			{
				std::string file = p.path().filename().string().substr(0, p.path().filename().string().size() - 4);
				this->styles.push_back(file);
			}
		}
	}
}

void ConfigWindow::UpdateValues()
{
	/* Program config */
	//this->config->SetConfig("auto_update", Utility::BoolToString(this->autoUpdate));
	this->config->SetConfig("start_tab", std::to_string(this->startTab));
	this->config->SetConfig("console",  Utility::BoolToString(this->console));
	this->config->SetConfig("style", this->styles[this->selectedStyle]);

	this->windowBorder = Utility::StringToBool(this->config->GetStyleVal("imgui_window_border_size"));
	this->childBorder  = Utility::StringToBool(this->config->GetStyleVal("imgui_child_border_size"));
	this->frameBorder  = Utility::StringToBool(this->config->GetStyleVal("imgui_frame_border_size"));
	this->tabBorder    = Utility::StringToBool(this->config->GetStyleVal("imgui_tab_border_size"));

	this->style.WindowBorderSize = this->windowBorder;
	this->style.ChildBorderSize = this->childBorder;
	this->style.FrameBorderSize = this->frameBorder;
	this->style.TabBorderSize = this->tabBorder;
}

void ConfigWindow::ResetConfig()
{
	Utility::AddLog("[ConfigTab] Resetting config...");

	this->config->LoadSystemConfig("config_backup.ini");
	this->config->LoadStyle();

	for (unsigned int i = 0; i < this->styles.size(); i++) //Set the selected save
		if (this->styles[i] == this->config->GetConfig("style")) this->selectedStyle = i;

	this->Load(this->config, this->resourceManager); //Load the new values
}

void ConfigWindow::SaveConfig()
{
	this->UpdateValues();
	this->config->Save();
	this->config->UpdateLanguage(this->language, false);
}

//Constructors/Destructors
ConfigWindow::ConfigWindow()
{

}

ConfigWindow::~ConfigWindow()
{

}

//Functions
void ConfigWindow::Update()
{
	if (!this->firstLoop)
	{
		this->ctupd1ColumnSize = this->config->GetBiggestLabelSize("ctupd1") + 15.0f; //Set the column size from ctupd1
		this->ctupd2ColumnSize = this->config->GetBiggestLabelSize("ctupd2") + 15.0f; //Set the column size from ctupd2
		this->firstLoop = true;
	}

	if (this->config && this->resourceManager)
	{
		ImGui::SetNextWindowPos(this->pos);
		ImGui::SetNextWindowSize(this->size);
		ImGui::Begin(this->config->GetLabel("ct_title").c_str(), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		this->ProgramConfig();

		ImGui::End();
	}
}

void ConfigWindow::Load(Config* config, Utility::ResourceManager* resourceManager)
{
	this->config = config;
	this->resourceManager = resourceManager;
	this->language = this->config->GetConfig("language");
	this->startTab = Utility::StringToInt(this->config->GetConfig("start_tab"));
	this->console = Utility::StringToBool(this->config->GetConfig("console"));

	//this->autoUpdate = Utility::StringToBool(this->config->GetConfig("auto_update"));

	strcpy(this->newTemplateName, this->config->GetConfig("style").c_str());

	this->windowBorder = Utility::StringToBool(this->config->GetStyleVal("imgui_window_border_size"));
	this->childBorder  = Utility::StringToBool(this->config->GetStyleVal("imgui_child_border_size"));
	this->frameBorder  = Utility::StringToBool(this->config->GetStyleVal("imgui_frame_border_size"));
	this->tabBorder    = Utility::StringToBool(this->config->GetStyleVal("imgui_tab_border_size"));

	this->LoadStyles();
	for (unsigned int i = 0; i < this->styles.size(); i++) //Set the selected save
		if (this->styles[i] == this->config->GetConfig("style")) this->selectedStyle = i;
}

void ConfigWindow::Save()
{
	this->SaveConfig();
	this->UpdateValues();
}