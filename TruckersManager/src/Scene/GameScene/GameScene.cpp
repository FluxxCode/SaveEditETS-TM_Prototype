#include "tmpch.h"

#include "GameScene.h"
#include "Application/Application.h"
#include "Scene/SaveEditScene/SaveEditScene.h"
#include "Scene/ConfigScene/ConfigScene.h"
#include "Scene/MoreScene/MoreScene.h"

//Functoins
void GameScene::Init()
{
	
}


void GameScene::UpdateMainSelection()
{
	ImGui::SetNextWindowPos(ImVec2(1280 / 2 - this->mainSelectionX / 2, 0));
	ImGui::Begin("mainselection", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	this->mainSelectionX = ImGui::GetWindowPos().x;

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_Button]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_Button]);
	ImGui::Button(this->GetConfig()->GetLabel("mt_game").c_str());
	ImGui::PopStyleColor(2);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected2);
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_save_edit").c_str()))
		this->LoadScene(new SaveEditScene(this->GetApp()));

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

//Constructors/Destructors
GameScene::GameScene(Application* app)
	: Scene(app)
{

}

GameScene::~GameScene()
{

}

//Functions
void GameScene::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->UpdateMainSelection();
	this->selectionWindow.Update(this->GetConfig(), this->GetResourceManager());
}

void GameScene::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameScene::OnLoad()
{
	Utility::AddLog("[GameScene] Loading scene: GameScene...");
	this->Init();
}

void GameScene::OnEnd()
{
	Utility::AddLog("[GameScene] Ending scene: GameScene...");
}