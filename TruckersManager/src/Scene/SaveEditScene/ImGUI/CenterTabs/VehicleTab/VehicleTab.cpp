#include "tmpch.h"
#include "VehicleTab.h"

ImVec4 VehicleTab::NumberToColor(unsigned int num)
{
	float mapped = Utility::Map(num - 20, 0, 100, 0, 510);
	float r = (255 -(mapped - 255)) / 255 ;
	float g = mapped / 255;

	if (mapped < 255)
		r = 1.0f;
	else if (mapped > 255)
		g = 1.0f;
	else if (mapped == 255)
	{
		r = 1.0f;
		g = 1.0f;
	}

	return ImVec4(r, g, 0.0f, 1.0f);
}

void VehicleTab::ColorRangeRectangle(unsigned int num)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, this->NumberToColor(num));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, this->NumberToColor(num));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, this->NumberToColor(num));
	ImGui::Button("  ");
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

//Constructors/Destructors
VehicleTab::VehicleTab()
{

}

VehicleTab::~VehicleTab()
{

}

//Functions
void VehicleTab::Update(Profile* profile)
{
	this->profile = profile;

	ImVec2 childSize = ImVec2(510.0f, 627.0f);
	ImVec2 imgButtonSize = ImVec2(32.0f, 32.0f);

	if (this->profile && this->config && this->resourceManager && this->consoleWindow)
	{
		ImGui::BeginChild("##child23453", childSize, true);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_truck").c_str());

		GLuint* img = nullptr;
		/* Truck */
		if (this->selectedTruck != "")
		{
			//Truck selection
			if (ImGui::BeginCombo("##truckselectioncombo", this->trucks->at(this->selectedTruck)->GetName().c_str()))
			{
				unsigned int count = 0;
				for (auto const& [key, val] : *this->trucks)
				{
					count++;

					if (this->profile->GetSave()->GetPlayerTruckID() == key)
						ImGui::PushStyleColor(ImGuiCol_Text, Utility::Colors::Green);

					std::string label = std::to_string(count) + ": " + this->trucks->at(key)->GetName();
					bool isSelected = (key == this->selectedTruck);
					if (ImGui::Selectable(label.c_str(), isSelected))
					{
						this->selectedTruck = key;
						this->truck = this->trucks->at(this->selectedTruck);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();

					if (this->profile->GetSave()->GetPlayerTruckID() == key)
						ImGui::PopStyleColor(1);
				}

				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair_all_trucks").c_str()))
			{
				for (auto const& [key, val] : *this->trucks)
				{
					*val->GetCabinDamage() = 0;
					*val->GetChassisDamage() = 0;
					*val->GetEngineDamage() = 0;
					*val->GetTiresDamage() = 0;
					*val->GetTransmissionDamage() = 0;
					*val->GetFuel() = 100;
				}
			}
			Utility::ImGuiToolTip(this->config->GetLabel("vt_btn_repair_all_trucks_tt").c_str());

			//Truck image
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\accessory\\" + this->truck->GetFileName() + ".png"); //Load the truck image

			ImGui::BeginChild("##truckimg", ImVec2(140.0f, 140.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Image((void*)(intptr_t)*img, ImVec2(128.0f, 128.0f)); //Render the truck image
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Text((this->config->GetLabel("vt_name") + " " + this->truck->GetName()).c_str());
			ImGui::Text((this->config->GetLabel("vt_id") + " " + this->truck->GetTruck()->id).c_str());

			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			img = this->resourceManager->LoadTexture("Resources\\icons\\Freepik\\wrench.png");
			if (ImGui::ImageButton((void*)(intptr_t)*img, imgButtonSize))
			{
				*this->truck->GetCabinDamage() = 0;
				*this->truck->GetChassisDamage() = 0;
				*this->truck->GetEngineDamage() = 0;
				*this->truck->GetTiresDamage() = 0;
				*this->truck->GetTransmissionDamage() = 0;
			}
			Utility::ImGuiToolTip(this->config->GetLabel("vt_btn_repair_all_tt"));

			ImGui::SameLine();

			img = this->resourceManager->LoadTexture("Resources\\icons\\Freepik\\fuel.png");
			if (ImGui::ImageButton((void*)(intptr_t)*img, imgButtonSize))
				*this->truck->GetFuel() = 100;
			Utility::ImGuiToolTip(this->config->GetLabel("vt_btn_refuel"));

			ImGui::EndGroup();

			//Fuel
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_fuel").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\adviser\\fuel.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the fuel image
			ImGui::SameLine();
			ImGui::SliderInt("##labelfuelint", this->truck->GetFuel(), 0, 100, "%d pct.");
			ImGui::SameLine();
			if (ImGui::Button(this->config->GetLabel("vt_btn_refuel").c_str()))
				*this->truck->GetFuel() = 100;
			ImGui::SameLine();
			this->ColorRangeRectangle(*this->truck->GetFuel());

			//Damage
			//Engine
			ImGui::NewLine();
			ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("vt_damage").c_str());
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_engine").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\engine.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the engine image
			ImGui::SameLine();
			ImGui::SliderInt("##labelengineint", this->truck->GetEngineDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(1);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->truck->GetEngineDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->truck->GetEngineDamage());

			//Transmission
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_transmission").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\transmission.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the engine image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltransmissionint", this->truck->GetTransmissionDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(2);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->truck->GetTransmissionDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->truck->GetTransmissionDamage());

			//Chassis
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_chassis").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\chassis.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the engine image
			ImGui::SameLine();
			ImGui::SliderInt("##labelchassisint", this->truck->GetChassisDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(3);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->truck->GetChassisDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->truck->GetChassisDamage());

			//Cabin
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_cabin").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\cabin.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the engine image
			ImGui::SameLine();
			ImGui::SliderInt("##labelcabinint", this->truck->GetCabinDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(4);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->truck->GetCabinDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->truck->GetCabinDamage());

			//Tyres
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_tires").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\tyres.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the engine image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltireint", this->truck->GetTiresDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(5);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->truck->GetTiresDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->truck->GetTiresDamage());
		}
		else
			ImGui::Text(this->config->GetLabel("vt_no_truck").c_str());

		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##child25438", childSize, true);

		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_trailer").c_str());

		/* Trailer*/
		if (this->selectedTrailer != "")
		{
			//Trailer selection
			if (ImGui::BeginCombo("##trailerselectioncombo", this->trailers->at(this->selectedTrailer)->GetName().c_str()))
			{
				unsigned int count = 0;
				for (auto const& [key, val] : *this->trailers)
				{
					count++;

					if (this->profile->GetSave()->GetPlayerTrailerID() == key)
						ImGui::PushStyleColor(ImGuiCol_Text, Utility::Colors::Green);

					std::string label = std::to_string(count) + ": " + this->trailers->at(key)->GetName();
					bool isSelected = (key == this->selectedTrailer);
					if (ImGui::Selectable(label.c_str(), isSelected))
					{
						this->selectedTrailer = key;
						this->trailer = this->trailers->at(this->selectedTrailer);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();

					if (this->profile->GetSave()->GetPlayerTrailerID() == key)
						ImGui::PopStyleColor(1);
				}

				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair_all_trailers").c_str()))
			{
				for (auto const& [key, val] : *this->trailers)
				{
					*val->GetChassisDamage() = 0;
					*val->GetBodyDamage() = 0;
					*val->GetTiresDamage() = 0;
					*val->GetCargoDamage() = 0;
				}
			}
			Utility::ImGuiToolTip(this->config->GetLabel("vt_btn_repair_all_trailers_tt").c_str()),

			/* Trailer Info */
			ImGui::BeginChild("##trailerinfo", ImVec2(492.0f, 140.0f), true);

			ImGui::Text((this->config->GetLabel("vt_trailer_mass") + " " + std::to_string((int)this->trailer->GetTrailerMass())).c_str());
			ImGui::Text((this->config->GetLabel("vt_cargo_mass") + " " + std::to_string((int)this->trailer->GetCargoMass())).c_str());
			ImGui::Text((this->config->GetLabel("vt_slave_trailer") + " " + std::to_string(this->trailer->GetSlaveTrailers()->size() + 1)).c_str());
			ImGui::Text((this->config->GetLabel("vt_id") + " " + this->trailer->GetTrailer()->id).c_str());

			ImGui::NewLine();

			img = this->resourceManager->LoadTexture("Resources\\icons\\Freepik\\wrench.png");
			if (ImGui::ImageButton((void*)(intptr_t)*img, imgButtonSize))
			{
				*this->trailer->GetBodyDamage() = 0;
				*this->trailer->GetChassisDamage() = 0;
				*this->trailer->GetTiresDamage() = 0;
				*this->trailer->GetCargoDamage() = 0;
			}
			Utility::ImGuiToolTip(this->config->GetLabel("vt_btn_repair_all_tt"));

			ImGui::EndChild();

			ImGui::NewLine();
			ImGui::TextColored(Utility::Colors::Green, this->config->GetLabel("vt_damage").c_str());
			//Damage
			//Body
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_body").c_str());
			GLuint* img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\trailer_body.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the body image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltrailerbody", this->trailer->GetBodyDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(1);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->trailer->GetBodyDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->trailer->GetBodyDamage());

			//Chassis
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_chassis").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\chassis.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render chassis image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltrailerchassis", this->trailer->GetChassisDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(2);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->trailer->GetChassisDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->trailer->GetChassisDamage());

			//Tires
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_tires").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\button\\tyres.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the tires image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltrailertires", this->trailer->GetTiresDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(3);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->trailer->GetTiresDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->trailer->GetTiresDamage());

			//Cargo
			ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("vt_cargo").c_str());
			img = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\adviser\\cargo.dds");
			ImGui::Image((void*)(intptr_t)*img, ImVec2(20.0f, 20.0f)); //Render the cargo image
			ImGui::SameLine();
			ImGui::SliderInt("##labeltrailercargo", this->trailer->GetCargoDamage(), 100, -1, "%d pct.");
			ImGui::SameLine();
			ImGui::PushID(3);
			if (ImGui::Button(this->config->GetLabel("vt_btn_repair").c_str()))
				*this->trailer->GetCargoDamage() = 0;
			ImGui::PopID();
			ImGui::SameLine();
			this->ColorRangeRectangle(100 - *this->trailer->GetCargoDamage());
		}
		else
			ImGui::Text(this->config->GetLabel("vt_no_trailer").c_str());

		ImGui::EndChild();
	}
}

void VehicleTab::Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow)
{
	//Set vars
	this->profile = profile;
	this->config = config;
	this->resourceManager = resourceManager;
	this->consoleWindow = consoleWindow;

	if (profile)
	{
		this->trucks = this->profile->GetSave()->GetTrucks();
		this->trailers = this->profile->GetSave()->GetTrailers();

		if (this->profile->GetSave()->GetTrucks()->size() > 0)
			this->selectedTruck = this->profile->GetSave()->GetTrucks()->begin()->first;
		if (this->profile->GetSave()->GetTrailers()->size() > 0)
			this->selectedTrailer = this->profile->GetSave()->GetTrailers()->begin()->first;

		if (this->profile->GetSave()->GetPlayerTruckID() != "null")
			this->selectedTruck = this->profile->GetSave()->GetPlayerTruckID();
		if (this->profile->GetSave()->GetPlayerTrailerID() != "null")			
			this->selectedTrailer = this->profile->GetSave()->GetPlayerTrailerID();

		if (this->selectedTruck != "")
			this->truck = this->profile->GetSave()->GetTrucks()->at(this->selectedTruck);
		if (this->selectedTrailer != "")
			this->trailer = this->profile->GetSave()->GetTrailers()->at(this->selectedTrailer);
	}
}

void VehicleTab::Save()
{
	//Truck
	for (auto const& [key, val] : *this->trucks)
		val->Save();

	for (auto const& [key, val] : *this->trailers)
		val->Save();
}