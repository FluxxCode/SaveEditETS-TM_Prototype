#include "tmpch.h"
#include "SkillTab.h"

//Functions
void SkillTab::LoadADR()
{
	std::string s = Utility::IntToBinary(Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["adr"])); //Convert the int into a binary number
	s = Utility::PadLeft(s, 6, '0');
	s = Utility::TurnString(s);

	for (unsigned int i = 0; i < s.size(); i++)
	{
		bool b = Utility::CharToBool(s[i]);
		this->adr[i] = b;
	}
}

//Constructors/Destructors
SkillTab::SkillTab()
{

}

SkillTab::~SkillTab()
{

}

//Functions
void SkillTab::Update()
{
	float btnSize = 47.8f;
	float btnImgSize = 40.0f;
	float firstChildX = 80.f, firstChildY = 80.0f;
	float secondChildX = 400.0f, seconChildY = 80.0f;
	float thirdChildX = 530.0f, thirdChildY = 80.0f;

	if (this->profile && this->config && this->resourceManager && this->consoleWindow)
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 90.0f);

		//ADR Skill
		ImGui::BeginChild("##child1238873", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		GLuint* img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_adr.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild(); 

		ImGui::NextColumn();
		ImGui::BeginChild("childadr23", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_adr").c_str());

		for (int i = 0; i < 6; i++)
		{
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\adr_" + std::to_string(i + 1) + ".dds");

			if (!this->adr[i]) //ADT is not selected
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(btnImgSize, btnImgSize))) this->adr[i] = true;
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}
			else 				
			{
				if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(btnImgSize, btnImgSize))) this->adr[i] = false;
			}

			ImGui::SameLine();
		}

		ImGui::EndChild();

#pragma region Distance skill
		//Distance skill
		ImGui::NextColumn();
		ImGui::BeginChild("##child32847", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_distance.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("childadr2663", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_distance").c_str());

		//Button none
		if (ImGui::Button(this->config->GetLabel("st_btn_distance_0").c_str(), ImVec2(btnSize, btnSize)))
		{
			for (unsigned int x = 0; x < this->distanceHovered.size(); x++) this->distanceHovered[x] = false; //Set the buttons infront of the button that is hoverd to true
			this->selectedDistance = 0;
		}

		ImGui::SameLine();

		//Distance level buttons
		for (int i = 0; i < 6; i++)
		{
			std::string str = "##btn" + std::to_string(i);
			if (this->distanceHovered[i])
			{
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->distanceHovered.size(); x++) this->distanceHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->distanceHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedDistance = i + 1;
				}
			}
			else 
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->distanceHovered.size(); x++) this->distanceHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->distanceHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedDistance = i + 1;
				}
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}

			ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		//Distance info
		ImGui::BeginChild("##child163987", ImVec2(thirdChildX, thirdChildY), true, ImGuiWindowFlags_NoScrollbar);

		//Skill
		std::string label = this->config->GetLabel("st_skill") + " " + std::to_string(this->selectedDistance);
		ImGui::TextColored(Utility::Colors::Blue, label.c_str());

		label = "st_distance_level_" + std::to_string(this->selectedDistance) + "_0";
		std::string txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());
		label = "st_distance_level_" + std::to_string(this->selectedDistance) + "_1";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());

		ImGui::EndChild();
		ImGui::NextColumn();

#pragma endregion

#pragma region High-Value Cargo 
		//High-Value Cargo 
		ImGui::BeginChild("##child3244847", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_heavy.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("childadr266983", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_heavy").c_str());

		//Button none
		if (ImGui::Button(this->config->GetLabel("st_btn_heavy_0").c_str(), ImVec2(btnSize, btnSize)))
		{
			for (unsigned int x = 0; x < this->heavyHovered.size(); x++) this->heavyHovered[x] = false; //Set the buttons infront of the button that is hoverd to true
			this->selectedHeavy = 0;
		}

		ImGui::SameLine();

		//Distance level buttons
		for (int i = 0; i < 6; i++)
		{
			std::string str = "##btnheavy" + std::to_string(i);
			if (this->heavyHovered[i])
			{
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->heavyHovered.size(); x++) this->heavyHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->heavyHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedHeavy = i + 1;
				}
			}
			else
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->heavyHovered.size(); x++) this->heavyHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->heavyHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedHeavy= i + 1;
				}
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}

			ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		//Distance info
		ImGui::BeginChild("##child1487", ImVec2(thirdChildX, thirdChildY), true, ImGuiWindowFlags_NoScrollbar);

		//Skill
		label = this->config->GetLabel("st_skill") + " " + std::to_string(this->selectedHeavy);
		ImGui::TextColored(Utility::Colors::Blue, label.c_str());

		label = "st_heavy_level_" + std::to_string(this->selectedHeavy) + "_0";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());
		label = "st_heavy_level_" + std::to_string(this->selectedHeavy) + "_1";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());

		ImGui::EndChild();
		ImGui::NextColumn();

#pragma endregion

#pragma region Fragile Cargo 
		//Fragile Cargo
		ImGui::BeginChild("##child32", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_fragile.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("childadr26", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_fragile").c_str());

		//Button none
		if (ImGui::Button(this->config->GetLabel("st_btn_fragile_0").c_str(), ImVec2(btnSize, btnSize)))
		{
			for (unsigned int x = 0; x < this->fragileHovered.size(); x++) this->fragileHovered[x] = false; //Set the buttons infront of the button that is hoverd to true
			this->selectedFragile = 0;
		}

		ImGui::SameLine();

		//Distance level buttons
		for (int i = 0; i < 6; i++)
		{
			std::string str = "##btnfragile" + std::to_string(i);
			if (this->fragileHovered[i])
			{
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->fragileHovered.size(); x++) this->fragileHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->fragileHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedFragile = i + 1;
				}
			}
			else
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->fragileHovered.size(); x++) this->fragileHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->fragileHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedFragile = i + 1;
				}
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}

			ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		//Info
		ImGui::BeginChild("##child187", ImVec2(thirdChildX, thirdChildY), true, ImGuiWindowFlags_NoScrollbar);

		//Skill
		label = this->config->GetLabel("st_skill") + " " + std::to_string(this->selectedFragile);
		ImGui::TextColored(Utility::Colors::Blue, label.c_str());

		label = "st_fragile_level_" + std::to_string(this->selectedFragile) + "_0";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());
		label = "st_fragile_level_" + std::to_string(this->selectedFragile) + "_1";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());

		ImGui::EndChild();
		ImGui::NextColumn();

#pragma endregion

#pragma region Just-In-Time Delivery Cargo 
		//Just-In-Time Delivery
		ImGui::BeginChild("##child322341", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_jit.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("childad34r26", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_urgent").c_str());

		//Button none
		if (ImGui::Button(this->config->GetLabel("st_btn_urgent_0").c_str(), ImVec2(btnSize, btnSize)))
		{
			for (unsigned int x = 0; x < this->urgentHovered.size(); x++) this->urgentHovered[x] = false; //Set the buttons infront of the button that is hoverd to true
			this->selectedUrgent = 0;
		}

		ImGui::SameLine();

		//Level buttons
		for (int i = 0; i < 6; i++)
		{
			std::string str = "##btnurgent" + std::to_string(i);
			if (this->urgentHovered[i])
			{
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->urgentHovered.size(); x++) this->urgentHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->urgentHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedUrgent = i + 1;
				}
			}
			else
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->urgentHovered.size(); x++) this->urgentHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->urgentHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedUrgent = i + 1;
				}
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}

			ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		//Info
		ImGui::BeginChild("##child169487", ImVec2(thirdChildX, thirdChildY), true, ImGuiWindowFlags_NoScrollbar);

		//Skill
		label = this->config->GetLabel("st_skill") + " " + std::to_string(this->selectedUrgent);
		ImGui::TextColored(Utility::Colors::Blue, label.c_str());

		label = "st_urgent_level_" + std::to_string(this->selectedUrgent) + "_0";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());
		label = "st_urgent_level_" + std::to_string(this->selectedUrgent) + "_1";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());
		label = "st_urgent_level_" + std::to_string(this->selectedUrgent) + "_2";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());

		ImGui::EndChild();
		ImGui::NextColumn();

#pragma endregion

#pragma region Eco-Driving
		//Eco-Driving
		ImGui::BeginChild("##child1234", ImVec2(firstChildX, 80.0f), true, ImGuiWindowFlags_NoScrollbar);
		img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\icon\\skill_mechanical.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(64.0f, 64.0f)); //Render the skill image
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("childad34ra26", ImVec2(secondChildX, seconChildY), true, ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("st_mechanical").c_str());

		//Button none
		if (ImGui::Button(this->config->GetLabel("st_btn_mechanical_0").c_str(), ImVec2(btnSize, btnSize)))
		{
			for (unsigned int x = 0; x < this->mechanicalHovered.size(); x++) this->mechanicalHovered[x] = false; //Set the buttons infront of the button that is hoverd to true
			this->selectedMechanical = 0;
		}

		ImGui::SameLine();

		//Level buttons
		for (int i = 0; i < 6; i++)
		{
			std::string str = "##btnmechanical" + std::to_string(i);
			if (this->mechanicalHovered[i])
			{
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->mechanicalHovered.size(); x++) this->mechanicalHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->mechanicalHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedMechanical = i + 1;
				}
			}
			else
			{
				ImGui::PushID(0);
				ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeactive);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnDeactivePressed);
				if (ImGui::Button(str.c_str(), ImVec2(btnSize, btnSize)))
				{
					for (unsigned int x = 0; x < this->mechanicalHovered.size(); x++) this->mechanicalHovered[x] = false; //Set every button to false
					for (int x = 0; x <= i; x++) this->mechanicalHovered[x] = true; //Set the buttons infront of the button that is hoverd to true
					this->selectedMechanical = i + 1;
				}
				ImGui::PopStyleColor(2);
				ImGui::PopID();
			}

			ImGui::SameLine();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		//Info
		ImGui::BeginChild("##child1694a87", ImVec2(thirdChildX, thirdChildY), true, ImGuiWindowFlags_NoScrollbar);

		//Skill
		label = this->config->GetLabel("st_skill") + " " + std::to_string(this->selectedMechanical);
		ImGui::TextColored(Utility::Colors::Blue, label.c_str());

		label = "st_mechanical_level_" + std::to_string(this->selectedMechanical) + "_0";
		txt = this->config->GetLabel(label);
		ImGui::Text(txt.c_str());

		ImGui::EndChild();
		ImGui::NextColumn();

#pragma endregion

		ImGui::Columns(1);
	}
}

void SkillTab::Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow)
{
	//Set vars
	this->profile = profile;
	this->config = config;
	this->resourceManager = resourceManager;
	this->consoleWindow = consoleWindow;

	this->adr.clear();
	for (int i = 0; i < 6; i++) this->adr.push_back(false);

	//Distance
	this->distanceHovered.clear();
	for (int i = 0; i < 6; i++) this->distanceHovered.push_back(false);

	//Load distance skill
	int adr = 0;
	adr = Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["long_dist"]); //Convert string to int
	for (int i = 0; i < adr; i++)	this->distanceHovered[i] = true;
	this->selectedDistance = adr;

	//Heavy
	this->heavyHovered.clear();
	for (int i = 0; i < 6; i++) this->heavyHovered.push_back(false);

	//Load High-Value Cargo skill
	int high = 0;
	high = Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["heavy"]); //Convert string to int
	for (int i = 0; i < high; i++)	this->heavyHovered[i] = true;
	this->selectedHeavy = high;

	//Fragile
	this->fragileHovered.clear();
	for (int i = 0; i < 6; i++) this->fragileHovered.push_back(false);

	//Load Fragile Cargo skill
	int fragile = 0;
	fragile = Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["fragile"]); //Convert string to int
	for (int i = 0; i < fragile; i++)	this->fragileHovered[i] = true;
	this->selectedFragile = fragile;

	//Urgent
	this->urgentHovered.clear();
	for (int i = 0; i < 6; i++) this->urgentHovered.push_back(false);

	//Load Urgent skill
	int urgent = 0;
	urgent = Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["urgent"]); //Convert string to int
	for (int i = 0; i < urgent; i++)	this->urgentHovered[i] = true;
	this->selectedUrgent = urgent;

	//Mechanical
	this->mechanicalHovered.clear();
	for (int i = 0; i < 6; i++) this->mechanicalHovered.push_back(false);

	//Load Mechanical skill
	int mechanical = 0;
	mechanical = Utility::StringToInt(this->profile->GetSave()->GetEconomyObj()->attributes["mechanical"]); //Convert string to int
	for (int i = 0; i < mechanical; i++)	this->mechanicalHovered[i] = true;
	this->selectedMechanical = mechanical;

	this->stupd1ColumnSize = this->config->GetBiggestLabelSize("stupd1");
	this->LoadADR();
}

void SkillTab::Save()
{
	this->profile->GetSave()->GetEconomyObj()->attributes["long_dist"] = std::to_string(this->selectedDistance);
	this->profile->GetSave()->GetEconomyObj()->attributes["heavy"] = std::to_string(this->selectedHeavy);
	this->profile->GetSave()->GetEconomyObj()->attributes["fragile"] = std::to_string(this->selectedFragile);
	this->profile->GetSave()->GetEconomyObj()->attributes["urgent"] = std::to_string(this->selectedUrgent);
	this->profile->GetSave()->GetEconomyObj()->attributes["mechanical"] = std::to_string(this->selectedMechanical);

	/* Save adr */
	int adrLevel = 0;
	for (unsigned int i = 0; i < this->adr.size(); i++)
		if (this->adr[i])	adrLevel += this->adrVal[i];

	this->profile->GetSave()->GetEconomyObj()->attributes["adr"] = std::to_string(adrLevel);
}