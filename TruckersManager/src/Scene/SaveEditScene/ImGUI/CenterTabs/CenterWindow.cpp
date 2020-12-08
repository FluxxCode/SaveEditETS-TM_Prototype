#include "tmpch.h"
#include "CenterWindow.h"

//Constructors/Destructors
CenterWindow::CenterWindow(ImVec2 pos, ImVec2 size)
	: pos(pos), size(size)
{

}

CenterWindow::~CenterWindow()
{

}

//Functions
void CenterWindow::Update()
{
    if (this->pos.x != -5.0f && this->pos.y != -5.0f)	ImGui::SetNextWindowPos(this->pos);
    if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);

    if (this->profile && this->config)
    {
        ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        if (ImGui::BeginTabBar("##tabs"))
        {
            if (ImGui::BeginTabItem(this->config->GetLabel("pt_tab").c_str()))
            {
                this->profileTab.Udpate();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(this->config->GetLabel("st_tab").c_str()))
            {
                this->skillTab.Update();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(this->config->GetLabel("vt_tab").c_str()))
            {
                this->vehicleTab.Update(this->profile);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

void CenterWindow::Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow)
{
    this->config = config;
    this->profileTab.LoadProfile(profile, config, resourceManager, consoleWindow);
    this->skillTab.Load(profile, config, resourceManager, consoleWindow);
    this->vehicleTab.Load(profile, config, resourceManager, consoleWindow);
}

void CenterWindow::Save()
{
    this->profileTab.Save();
    this->skillTab.Save();
    this->vehicleTab.Save();
}