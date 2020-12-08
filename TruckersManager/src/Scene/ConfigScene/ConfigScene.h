#pragma once

#include "Scene/Scene.h"
#include "ImGUI/ConfigWindow/ConfigWindow.h"

/*
	Scene to load the ETS profile
*/

namespace fs = std::filesystem;

class ConfigScene : public Scene
{
private:
	//Variables
	ConfigWindow configWindow = ConfigWindow();

	int mainSelectionX = 0;

	//Functions
	void Init();

	void UpdateMainSelection();

public:
	//Constructors/Destructors
	ConfigScene(Application* app);
	virtual ~ConfigScene();

	//Functions
	void Update();
	void Render();
	void OnLoad();
	void OnEnd();
};