#pragma once

#include "Scene/GameScene/ImGUI/RealtimeEditingWindow/RealtimeEditingWindow.h"
#include "Scene/GameScene/ImGUI/AppsWindow/AppsWindow.h"

class SelectionWindow
{
private:
	//Variables
	ImVec2 pos		  = ImVec2(10.0f, 35.0f);
	ImVec2 size		  = ImVec2(250.0f, 675.0f);
	ImVec2 childPos   = ImVec2(265.0f, 35.0f);
	ImVec2 childSize  = ImVec2(1005.0f, 675.0f);
	ImVec2 buttonSize = ImVec2(50.0f, 20.0f);

	Config* config = nullptr;
	Utility::ResourceManager* resouceManager = nullptr;

	bool calculatedSizes = false;

	std::vector<std::string> windowNameLables = { "sw_btn_realtime_editing", "sw_btn_apps" };
	int selectedWindow = 0;

	//Windows
	RealtimeEditingWindow realtimeEditingWindow;
	AppsWindow appsWindow;

	//Functions
	void CalculateSizes();

public:
	//Constructors/Destructors
	SelectionWindow();
	virtual ~SelectionWindow();

	//Functions
	void Update(Config* config, Utility::ResourceManager* resourceManager);
};