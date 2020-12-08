#include "tmpch.h"
#include "ProfileTab.h"

//Functions
void ProfileTab::Init()
{
	//Load textures
	//Female
	this->femaleIcon = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\female_icon.dds");
	if (this->femaleIcon == 0) //Error occured
	{
		Utility::AddLog("[ProfileTab] ERROR: Unable to load: Resources\\ets\\material\\ui\\female_icon.dds");
		this->consoleWindow->PushMessage("(#err#)ERROR::ProfileTab: Unable to load: Resources\\ets\\material\\ui\\female_icon.dds");
	}

	//Male
	this->maleIcon = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\male_icon.dds");
	if (this->maleIcon == 0) //Error occured
	{
		Utility::AddLog("[ProfileTab] ERROR: Unable to load: Resources\\ets\\material\\ui\\female_icon.dds");
		this->consoleWindow->PushMessage("(#err#)ERROR::ProfileTab: Unable to load: Resources\\ets\\material\\ui\\male_icon.dds");
	}
}

void ProfileTab::UpdateWindow()
{
	this->firstColumnSize = this->config->GetBiggestLabelSize("ptupd1") + 15.0f;

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, this->firstColumnSize);

	//Profile name
	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pt_profile_name").c_str());
	ImGui::NextColumn();
	ImGui::InputText("##pname", this->profileName, 20);
	ImGui::NextColumn();

	//Company name
	ImGui::TextColored(Utility::Colors::Blue, config->GetLabel("pt_company_name").c_str());
	ImGui::NextColumn();
	ImGui::InputText("##cname", this->companyName, 20);
	ImGui::NextColumn();

	//Gender
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("pt_gender").c_str());
	ImGui::NextColumn();

	//Female
	if (this->femaleIcon)	ImGui::Image((void*)(intptr_t)*this->femaleIcon, ImVec2(20.0f, 20.0f)); //Render the female icon
	ImGui::SameLine();
	ImGui::Checkbox("##femalech", &female);
	if (this->female) this->male = false;

	ImGui::SameLine(0.0f, 40.0f);

	//Male
	if (this->maleIcon)	ImGui::Image((void*)(intptr_t)*this->maleIcon, ImVec2(20.0f, 20.0f)); //Render the male icon
	ImGui::SameLine();
	ImGui::Checkbox("##malech", &male);
	if (this->male) this->female = false;

	ImGui::Columns(1);

	/* Update image selections */
	this->ProfileImages();
	ImGui::SameLine();

	this->CompanyImages();
	ImGui::SameLine();

	this->BrandImages();
}

void ProfileTab::UpdateValues()
{
	//Udpate values
	if (this->profileName != this->profile->GetAttribute("profile_name"))	this->profile->SetAttribute("profile_name", this->profileName);
	if (this->companyName != this->profile->GetAttribute("company_name"))	this->profile->SetAttribute("company_name", this->companyName);
	this->profile->SetDriverImg(this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\driver\\" + this->selectedProfileIcon));
	this->profile->SetLogoImg(this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\player_logo\\" + this->selectedLogo));
	this->profile->SetBrandImg(this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\accessory\\" + this->selectedBrand));
}

void ProfileTab::ProfileImages()
{
	ImGui::BeginGroup();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("pt_profile_picture").c_str());
	ImGui::BeginChild("##truckpicture", ImVec2(this->winX, this->winY), true);

	std::string path = "Resources\\ets\\material\\ui\\driver";
	int i = 0;
	for (auto& p : fs::directory_iterator(path))
	{
		if (i % 4 != 0) ImGui::SameLine(); //New line every 4 items

		GLuint* img = this->resourceManager->LoadTexture(p.path().string()); //Load image
		if (!img)
		{
			Utility::AddLog("[ProfileTab] ERROR: Unablel to load: \"" + path + ".dds");
			this->consoleWindow->PushMessage("(#err#)ERROR::ProfileTab: Unable to load: \"" + path + "\"");
		}

		ImGui::PushID(0);
		if (this->selectedProfileIcon != p.path().filename().string())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnSelected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}

		if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(this->picX, this->picY))) this->selectedProfileIcon = p.path().filename().string();

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		i++;
	}

	ImGui::EndChild();
	ImGui::EndGroup();
}

void ProfileTab::CompanyImages()
{
	ImGui::BeginGroup();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("pt_company_logo").c_str());
	ImGui::BeginChild("##companylogo", ImVec2(this->winX, this->winY), true);

	std::string path = "Resources\\ets\\material\\ui\\player_logo";
	int i = 0;
	for (auto& p : fs::directory_iterator(path))
	{
		if (i % 4 != 0) ImGui::SameLine(); //New line every 4 items

		GLuint* img = this->resourceManager->LoadTexture(p.path().string()); //Load image
		if (!img)
		{
			Utility::AddLog("[ProfileTab] ERROR: Unablel to load: \"" + path + "\"");
			this->consoleWindow->PushMessage("(#err#)ERROR::ProfileTab: Unable to load: \"" + path +"\"");
		}

		ImGui::PushID(0);
		if (this->selectedLogo != p.path().filename().string())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnSelected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}

		if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(this->picX, this->picY))) this->selectedLogo = p.path().filename().string();

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		i++;
	}

	ImGui::EndChild();
	ImGui::EndGroup();
}

void ProfileTab::BrandImages()
{
	ImGui::BeginGroup();
	ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("pt_brand").c_str());
	ImGui::BeginChild("##truckimgsels", ImVec2(this->winX, this->winY), true);

	std::string path = "Resources\\ets\\material\\ui\\accessory";
	int i = 0;
	for (auto& p : fs::directory_iterator(path))
	{
		if (i % 4 != 0) ImGui::SameLine(); //New line every 4 items

		GLuint* img = this->resourceManager->LoadTexture(p.path().string()); //Load image
		if (!img)
		{
			Utility::AddLog("[ProfileTab] ERROR: Unablel to load: \"" + path + "\"");
			this->consoleWindow->PushMessage("(#err#)ERROR::ProfileTab: Unable to load: \"" + path + "\"");
		}

		ImGui::PushID(0);
		if (this->selectedBrand != p.path().filename().string())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnSelected); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Utility::Colors::btnHovered); //Change color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Utility::Colors::btnActive); //Change color
		}

		if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(this->picX, this->picY))) 
			this->selectedBrand = p.path().filename().string();

		ImGui::PopStyleColor(3);
		ImGui::PopID();
		i++;
	}

	ImGui::EndChild();
	ImGui::EndGroup();
}

//Constructors/Destructors
ProfileTab::ProfileTab()
	: profile(profile)
{

}

ProfileTab::~ProfileTab()
{

}

//Functions
void ProfileTab::Udpate()
{
	if (this->profile && this->config && this->resourceManager)
	{
		this->UpdateWindow();
		this->UpdateValues();
	}
}

void ProfileTab::LoadProfile(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow)
{
	this->profile = profile;
	this->config = config;
	this->resourceManager = resourceManager;
	this->consoleWindow = consoleWindow;

	std::string str = profile->GetAttribute("profile_name");
	this->profileName = new char[str.length() + 1];
	strcpy(this->profileName, str.c_str());

	str = profile->GetAttribute("company_name");
	this->companyName = new char[str.length() + 1];
	strcpy(this->companyName, str.c_str());

	if (profile->GetAttribute("male") == "true")
	{
		this->female = false;
		this->male   = true;
	}

	this->selectedProfileIcon = profile->GetAttribute("face") + ".dds";
	this->selectedLogo = profile->GetAttribute("logo") + ".png";
	this->selectedBrand = profile->GetAttribute("brand") + ".png";

	this->Init();
}

void ProfileTab::Save()
{
	//Check if no other profile has the name
	int num = 0;
Check:
	std::string pathHex = fs::path(this->profile->GetPath()).parent_path().string() + "\\" + Utility::StringToHex(this->profileName);
	if (pathHex != this->profile->GetPath()) //Changed the profile name
	{
		if (fs::exists(pathHex))
		{
			std::string str = this->profileName + std::to_string(num);
			strcpy(this->profileName, str.c_str());
			num++;
			goto Check; //Check again
		}
	}

	this->profile->SetAttribute("profile_name", this->profileName);
	this->profile->SetAttribute("company_name", this->companyName);
	if (this->male)	this->profile->SetAttribute("male", "true");
	else			this->profile->SetAttribute("male", "false");
	this->profile->SetAttribute("face", this->selectedProfileIcon.substr(0, this->selectedProfileIcon.find(".")));
	this->profile->SetAttribute("logo", this->selectedLogo.substr(0, this->selectedLogo.find(".")));
	this->profile->SetAttribute("brand", this->selectedBrand.substr(0, this->selectedBrand.find(".")));
}