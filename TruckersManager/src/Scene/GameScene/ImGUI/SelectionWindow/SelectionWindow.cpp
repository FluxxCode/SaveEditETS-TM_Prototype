#include "tmpch.h"
#include "SelectionWindow.h"

//Functions
void SelectionWindow::CalculateSizes()
{
	this->calculatedSizes = true;

	int btnMaxSize = this->config->GetBiggestLabelSize("swupd1");
	if (btnMaxSize < 75)
		btnMaxSize = 75;
	else if (btnMaxSize > 200)
		btnMaxSize = 200;

	this->buttonSize.x = btnMaxSize + 10.0f;

	this->pos.x  = 10.0f;
	this->pos.y  = 53.0f;
	this->size.x = btnMaxSize + 25.0f;
	this->size.y = 657.0f;

	this->childPos.x  = 10.0f + this->size.x + 5.0f;
	this->childPos.y  = 35.0f;
	this->childSize.x = 1280 - 25.0f - this->size.x;
	this->childSize.y = 675.0f;
}

//Constructors/Destructors
SelectionWindow::SelectionWindow()
{

}

SelectionWindow::~SelectionWindow()
{

}

//Functions
void SelectionWindow::Update(Config* config, Utility::ResourceManager* resourceManager)
{
	this->config = config;
	this->resouceManager = resouceManager;

	if (!this->calculatedSizes)
		this->CalculateSizes();

	ImGui::SetNextWindowPos(this->pos);
	ImGui::SetNextWindowSize(this->size);
	ImGui::Begin("##selectionwindow", 0, ImGuiWindowFlags_NoDecoration);
	
	ImGuiStyle& style = ImGui::GetStyle();
	int selected = this->selectedWindow;
	for (int i = 0; i < this->windowNameLables.size(); i++)
	{
		if (selected != i)
			ImGui::PushStyleColor(ImGuiCol_Button, Utility::Colors::btnDeselected);
		else
		{
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  style.Colors[ImGuiCol_Button]);
		}

		if (ImGui::Button(this->config->GetLabel(this->windowNameLables[i]).c_str(), this->buttonSize))
			this->selectedWindow = i;

		if (selected != i)
			ImGui::PopStyleColor(1);
		else
			ImGui::PopStyleColor(2);
	}

	if (this->selectedWindow == 0)		this->realtimeEditingWindow.Update(config, resourceManager, this->childPos, this->childSize);
	else if (this->selectedWindow == 1) this->appsWindow.Update(config, resourceManager, this->childPos, this->childSize);

	ImGui::End();
}