#include "tmpch.h"
#include "CreditsWindow.h"

//Constructors/Destructors
CreditsWindow::CreditsWindow(ImVec2 pos, ImVec2 size)
	: pos(pos), size(size)
{

}

CreditsWindow::~CreditsWindow()
{

}

//Functions
void CreditsWindow::Udpate(Config* config, bool* bOpen)
{
	if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);

	ImGui::SetNextWindowBgAlpha(1);

	ImGui::Begin(config->GetLabel("c_title").c_str(), bOpen, ImGuiWindowFlags_NoResize);
	ImGui::Separator();

	ImGui::BeginChild("##txt", ImVec2(ImGui::GetWindowSize().x - 15.0f, ImGui::GetWindowSize().y - 45.0f));

	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("c_icons").c_str());
	ImGui::Text(config->GetLabel("c_icon_1").c_str());

	ImGui::EndChild();

	ImGui::Separator();
	ImGui::End();
}