#include "tmpch.h"
#include "CompanyWindow.h"

//Constructors/Destructors
CompanyWindow::CompanyWindow(Profile* profile, ImVec2 pos, ImVec2 size)
	: profile(NULL), pos(pos), size(size), money(0)
{

}

CompanyWindow::~CompanyWindow()
{

}

//Functions
void CompanyWindow::Update(Config* config)
{
#ifndef LLONG_MIN
	ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

	if (this->pos.x != -5.0f && this->pos.y != -5.0f)	ImGui::SetNextWindowPos(this->pos);
	if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);

	ImGui::Begin(config->GetLabel("cw_title").c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (this->profile)
	{
		ImGui::Text(this->profile->GetAttribute("company_name").c_str());
		ImGui::BeginChild("##ImgLogo", ImVec2(89.0f, 89.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		//Logo Image
		GLuint img = *this->profile->GetLogoImg();
		if (img)	ImGui::Image((void*)(intptr_t)img, ImVec2(74.0f, 74.0f)); //Render the drive image
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("##ImgBrand", ImVec2(89.0f, 89.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		//Lgo Image
		img = *this->profile->GetBrandImg();
		if (img)	ImGui::Image((void*)(intptr_t)img, ImVec2(74.0f, 74.0f)); //Render the drive image
		ImGui::EndChild();

		ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("cw_money").c_str());

		/* Money input */
		const ImU64 step = 1000;
		static bool inputs_step = true;
		ImGui::SetNextItemWidth(215.0f);
		ImGui::InputScalar("##inputmoney", ImGuiDataType_U64, &this->money, inputs_step ? &step : NULL);
		if (this->money > ULLONG_MAX)	this->money = ULLONG_MAX;
		if (this->money < 0)			this->money = 0;

		ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("cw_hq").c_str());
		ImGui::Text(this->profile->GetSave()->GetPlayerObj()->attributes["hq_city"].c_str());

		ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("cw_trucks").c_str());
		ImGui::Text(std::to_string(this->profile->GetSave()->GetPlayerObj()->arraySizes["trucks"]).c_str());

		ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("cw_trailers").c_str());
		ImGui::Text(std::to_string(this->profile->GetSave()->GetPlayerObj()->arraySizes["trailers"]).c_str());

		ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("cw_drivers").c_str());
		ImGui::Text(std::to_string(this->profile->GetSave()->GetPlayerObj()->arraySizes["drivers"]).c_str());
	}

	ImGui::End();
}

void CompanyWindow::Load(ConsoleWindow* consoleWindow)
{
	std::string m = this->profile->GetSave()->GetBankObj()->attributes["money_account"].c_str();

	this->money = Utility::StringToImU64(m);
	if (this->money < 0)
	{
		consoleWindow->PushMessage("(#err#)ERROR::Utility:StringToInt: money_account: " + m + " Error: " + std::to_string((int)this->money));
		this->money = 0;
	}
	if (this->money < 0)	this->money = 0;
}

void CompanyWindow::Save()
{
	this->profile->GetSave()->GetBankObj()->attributes["money_account"] = std::to_string(this->money);
}