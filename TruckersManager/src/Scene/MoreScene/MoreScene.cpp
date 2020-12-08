#include "tmpch.h"

#include "MoreScene.h"
#include "Application/Application.h"
#include "Scene/GameScene/GameScene.h"
#include "Scene/SaveEditScene/SaveEditScene.h"
#include "Scene/ConfigScene/ConfigScene.h"

//Functoins
void MoreScene::Init()
{
	
}


void MoreScene::UpdateMainSelection()
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

	ImGui::SameLine();
	if (ImGui::Button(this->GetConfig()->GetLabel("mt_config").c_str()))
		this->LoadScene(new ConfigScene(this->GetApp()));
	ImGui::PopStyleColor(1);

	ImGui::SameLine();

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_Button]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_Button]);
	ImGui::Button(this->GetConfig()->GetLabel("mt_more").c_str());
	ImGui::PopStyleColor(2);

	ImGui::SameLine();
	ImGui::Text(("V" + this->GetApp()->GetAppVersion()).c_str());

	ImGui::End();
}

void MoreScene::UpdateHelp()
{
	ImGui::Text("Comming in a later update...");
}

void MoreScene::UpdatePatchnotes()
{
	std::string str;
	if (fs::exists("patchnotes.txt"))
	{
		std::ifstream ifs("patchnotes.txt");
		str = std::string((std::istreambuf_iterator<char>(ifs)),
						  (std::istreambuf_iterator<char>()));
	}
	else 
	{
		if (!this->err)
			Utility::AddLog("[MoreScene] ERROR: Unable to open file: patchnotes.txt");

		str = "ERROR: Unable to open file: patchnotes.txt";
		this->err = true;
	}

	ImGui::Text(str.c_str());
}

void MoreScene::UpdateCredits()
{
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("ms_credits_icons").c_str());
	ImGui::Text(this->config->GetLabel("ms_credits_icon_1").c_str());
	
	for (const auto& dir : fs::recursive_directory_iterator("Resources\\Icons\\Freepik"))
	{
		if (!dir.is_directory())
		{
			GLuint* img = this->GetResourceManager()->LoadTexture(dir.path().string());
			ImGui::Image((void*)(intptr_t)*img, ImVec2(16.0f, 16.0f));
			ImGui::SameLine();
		}
	}
}

//Constructors/Destructors
MoreScene::MoreScene(Application* app)
	: Scene(app), config(app->GetConfig())
{

}

MoreScene::~MoreScene()
{

}

//Functions
void MoreScene::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->UpdateMainSelection();

	ImGui::SetNextWindowPos(this->pos);
	ImGui::SetNextWindowSize(this->size);
	ImGui::Begin(this->config->GetLabel("ms_title").c_str(), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginTabBar("##maintabbar"))
	{
		//Help
		if (ImGui::BeginTabItem(this->config->GetLabel("ms_help").c_str()))
		{
			this->UpdateHelp();
			ImGui::EndTabItem();
		}
		//Patchnotes
		if (ImGui::BeginTabItem(this->config->GetLabel("ms_patchnotes").c_str()))
		{
			this->UpdatePatchnotes();
			ImGui::EndTabItem();
		}
		//Credits
		if (ImGui::BeginTabItem(this->config->GetLabel("ms_credits").c_str()))
		{
			this->UpdateCredits();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void MoreScene::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MoreScene::OnLoad()
{
	Utility::AddLog("[MoreScene] Loading scene: MoreScene...");
	this->Init();
}

void MoreScene::OnEnd()
{
	Utility::AddLog("[MoreScene] Ending scene: GameScene...");
}