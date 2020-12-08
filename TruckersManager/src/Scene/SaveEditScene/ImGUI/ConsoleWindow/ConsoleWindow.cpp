#include "tmpch.h"
#include "ConsoleWindow.h"

//Functions
void ConsoleWindow::Print()
{

}

void ConsoleWindow::ExecuteCommand(std::string command)
{
	this->PushMessage("(#in#)>> " + command);

	//Clear
	if (command == "clear")		this->infos.clear();

	//Maxinfos
	else if (command.find("maxinfos=") != std::string::npos)
	{
		int x = 0;
		std::string num = command.substr(9, command.size());
		try { x = std::stoi(num); }
		catch (std::invalid_argument const& e) { this->PushMessage("(#err#)ERROR: " + num + " std::invalid_argument: " + e.what()); return; }
		catch (std::out_of_range const& e) { this->PushMessage("(#err#)ERROR:" + num + " Integer overflow: std::out_of_range: " + e.what()); return; }
		this->maxInfos = x;
		this->PushMessage("maxinfos=" + std::to_string(this->maxInfos));
	}
	else
		this->PushMessage("(#war#)Command: \"" + command + "\" not found!");
}

//Constrcutors/Destructors
ConsoleWindow::ConsoleWindow(ImVec2 pos, ImVec2 size)
	: pos(pos), size(size)
{

}

ConsoleWindow::~ConsoleWindow()
{

}

//Functions
void ConsoleWindow::Update()
{
	if (this->pos.x != -5.0f && this->pos.y != -5.0f)	ImGui::SetNextWindowPos(this->pos);
	if (this->size.x != -5.0f && this->size.y != -5.0f)	ImGui::SetNextWindowSize(this->size);

	ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (this->infos.size() > this->maxInfos) //Delete infos if max infos is reached
		for (unsigned int i = 0; i < this->infos.size() - this->maxInfos; i++)
			this->infos.erase(this->infos.begin(), this->infos.begin() + 1);

	ImGui::SetNextItemWidth(100.0f);
	ImGui::BeginChild("Console output", ImVec2(720.0f, ImGui::GetWindowSize().y - 35.0f), true); //Begin console output child
    for (unsigned int i = 0; i < this->infos.size(); i++) //Print conosle
    {
        std::string item = this->infos[i].c_str();
		
		if (item.find("(#err#)") != std::string::npos)
		{
			if (this->showErrors)
			{
				item.erase(0, 7);
				ImGui::TextColored(Utility::Colors::DarkOrange, item.c_str());
			}
		}
		else if (item.find("(#war#)") != std::string::npos)
		{
			if (this->showWarnings)
			{
				item.erase(0, 7);
				ImGui::TextColored(Utility::Colors::Yellow, item.c_str());
			}
		}
		else if (item.find("(#in#)") != std::string::npos)
		{
			if (this->showInput)
			{
				item.erase(0, 6);
				ImGui::Text(item.c_str());
			}
		}
		else if (this->showInfos)
			ImGui::Text(item.c_str());
    }
	ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild(); //End console output child

	ImGui::SameLine();

	ImGui::BeginChild("Right side", ImVec2(300.0f, ImGui::GetWindowSize().y - 35.0f), true); //Begin button, input child

	/* Buttons */
	ImGui::Text("Show: ");
	ImGui::SameLine();
	//Infos
	if (!this->showInfos)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.101f, 0.152f, 0.239f, 1.0f)); //Change color
		if (ImGui::Button("Infos"))			this->showInfos = !this->showInfos;
		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}
	else
		if (ImGui::Button("Infos"))			this->showInfos = !this->showInfos;

	ImGui::SameLine();

	//Warnings
	if (!this->showWarnings)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.101f, 0.152f, 0.239f, 1.0f)); //Change color
		if (ImGui::Button("Warnings"))		this->showWarnings = !this->showWarnings;
		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}
	else
		if (ImGui::Button("Warnings"))		this->showWarnings = !this->showWarnings;

	ImGui::SameLine();

	//Errors
	if (!this->showErrors)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.101f, 0.152f, 0.239f, 1.0f)); //Change color
		if (ImGui::Button("Errors"))		this->showErrors = !this->showErrors;
		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}
	else
		if (ImGui::Button("Errors"))		this->showErrors = !this->showErrors;

	ImGui::SameLine();

	//Input
	if (!this->showInput)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.101f, 0.152f, 0.239f, 1.0f)); //Change color
		if (ImGui::Button("Input"))		this->showInput = !this->showInput;
		ImGui::PopStyleColor(1);
		ImGui::PopID();
	}
	else
		if (ImGui::Button("Input"))		this->showInput = !this->showInput;

	/* Input */
	ImGui::Text("Input:");
	ImGui::SameLine();
	bool reclaimFocus = false;
	char buf[300] = { 0 };
	ImGui::SetNextItemWidth(225.0f);
	if (ImGui::InputText("##Input", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		this->ExecuteCommand(buf);
		reclaimFocus = true;
	}
	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::EndChild(); //End right side

	ImGui::End();
}

void ConsoleWindow::PushMessage(std::string message)
{
	this->infos.push_back(message);
}