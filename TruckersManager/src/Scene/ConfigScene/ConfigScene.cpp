#include "tmpch.h"

#include "ConfigScene.h"
#include "Application/Application.h"
#include "Scene/GameScene/GameScene.h"
#include "Scene/SaveEditScene/SaveEditScene.h"
#include "Scene/MoreScene/MoreScene.h"

//Functoins
void ConfigScene::Init()
{
	
}


void ConfigScene::UpdateMainSelection()
{
	ImGui::SetNextWindowPos(ImVec2(1280 / 2 - this->mainSelectionX / 2, 0));
	ImGui::Begin("mainselection", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	this->mainSelectionX = ImGui::GetWindowPos().x;

	ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected2);
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_game").c_str()))
		this->LoadScene(new GameScene(this->GetApp()));

	ImGui::SameLine();

	if (ImGui::Button(this->GetConfig()->GetLabel("mt_save_edit").c_str()))
		this->LoadScene(new SaveEditScene(this->GetApp()));
	ImGui::PopStyleColor(1);

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_Button]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_Button]);
	ImGui::SameLine();
	ImGui::Button(this->GetConfig()->GetLabel("mt_config").c_str());
	ImGui::PopStyleColor(2);

	ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected2);
	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_more").c_str()))
		this->LoadScene(new MoreScene(this->GetApp()));
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::Text(("V" + this->GetApp()->GetAppVersion()).c_str());

	ImGui::End();
}

//Constructors/Destructors
ConfigScene::ConfigScene(Application* app)
	: Scene(app)
{

}

ConfigScene::~ConfigScene()
{

}

//Functions
void ConfigScene::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->UpdateMainSelection();
	this->configWindow.Update();
}

void ConfigScene::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ConfigScene::OnLoad()
{
	Utility::AddLog("[ConfigScene] Loading scene: ConfigScene...");
	this->Init();
	this->configWindow.Load(this->GetApp()->GetConfig(), this->GetApp()->GetResourceManager());
}

void ConfigScene::OnEnd()
{
	Utility::AddLog("[ConfigScene] Ending scene: ConfigScene...");
	this->configWindow.Save();
}