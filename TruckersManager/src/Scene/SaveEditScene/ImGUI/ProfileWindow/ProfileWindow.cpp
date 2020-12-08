#include "tmpch.h"
#include "ProfileWindow.h"

//Constructors/Destcutors
ProfielWindow::ProfielWindow(Profile* profile, ImVec2 pos, ImVec2 size)
	: profile(NULL), pos(pos), size(size)
{

}

ProfielWindow::~ProfielWindow()
{

}

void ProfielWindow::Update(Config* config)
{
#ifndef LLONG_MIN
	ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

	if (pos.x != -5.0f && pos.y != -5.0f)	ImGui::SetNextWindowPos(pos);
	if (size.x != -5.0f && size.y != -5.0f)	ImGui::SetNextWindowSize(size);

	ImGui::Begin(config->GetLabel("pw_title").c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (!this->profile)
		return;
	if (!this->profile->GetSave())
		return;

	/* Images */
	ImGui::BeginChild("ProfileImage", ImVec2(ImGui::GetWindowSize().x / 2 - 10, 100), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); //------- Begin child
	//Driver Image
	GLuint* img = this->profile->GetDriverImg();
	if (img)	ImGui::Image((void*)(intptr_t)*img, ImVec2(96.0f, 96.0f)); //Render the drive image

	ImGui::EndChild(); //------- End child

	ImGui::SameLine();

	ImGui::BeginGroup(); //------- Begin group

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_profile").c_str());
	ImGui::Text(this->profile->GetAttribute("profile_name").c_str());
	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_save").c_str());
	ImGui::Text(this->profile->GetSaveDisplayStr().c_str());

	if (ImGui::Button(config->GetLabel("pw_btn_change").c_str(), ImVec2(100.0f, 20.0f)))
	{
		Utility::AddLog("[ProfileWindow] Button: Change");
		this->btnChangeProfileCallback();
		ImGui::EndGroup();
		ImGui::End();
		return;
	}

	Utility::ImGuiToolTip(config->GetLabel("pw_btn_change_tt"));

	ImGui::EndGroup(); //------- End group

	ImGui::Checkbox(config->GetLabel("pw_create_backup").c_str(), &this->backup);
	Utility::ImGuiToolTip(config->GetLabel("pw_create_backup_tt"));

	if (ImGui::Button(config->GetLabel("pw_btn_save").c_str(), ImVec2(215.0f, 20.0f)))
	{
		Utility::AddLog("[ProfileWindow] Button: Save - Backups: " + Utility::BoolToString(this->backup));
		this->btnSaveCallback();
	}
	Utility::ImGuiToolTip(config->GetLabel("pw_btn_save_tt"));
		
	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_ep").c_str());

	/* XP input */
	const ImU64 step = 1000;
	static bool inputs_step = true;
	ImGui::SetNextItemWidth(215.0f);
	ImGui::InputScalar("##input-xp", ImGuiDataType_U64, &this->xp, inputs_step ? &step : NULL);
	Utility::ImGuiToolTip(config->GetLabel("pw_ep_tt"));
	if (this->xp > 9999999)	this->xp = 9999999;
	if (this->xp < 1000)	this->xp = 1000;

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_visited_cities").c_str());
	ImGui::Text(std::to_string(this->profile->GetSave()->GetEconomyObj()->arraySizes["visited_cities_count"]).c_str()); //Visited cities

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_unlocked_dealers").c_str());
	ImGui::Text(std::to_string(this->profile->GetSave()->GetEconomyObj()->arraySizes["unlocked_dealers"]).c_str());

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pw_total_distance").c_str());
	ImGui::Text(this->profile->GetSave()->GetEconomyObj()->attributes["total_distance"].c_str());
	ImGui::End();
}

void ProfielWindow::Load(ConsoleWindow* consoleWindow)
{
	std::string m = this->profile->GetSave()->GetEconomyObj()->attributes["experience_points"].c_str();
	this->xp = Utility::StringToImU64(m);
	if (this->xp < 0)
	{
		consoleWindow->PushMessage("(#err#)ERROR::Utility:StringToInt: experience_points: " + m + " Error: " + std::to_string((int)this->xp));
		this->xp = 0;
	}
	if		(this->xp > 9999999) this->xp = 9999999;
	else if (this->xp == 0)	     this->xp = 0;
}

void ProfielWindow::Save()
{
	this->profile->GetSave()->GetEconomyObj()->attributes["experience_points"] = std::to_string(this->xp);
}