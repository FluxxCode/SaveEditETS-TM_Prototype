#pragma once

#include "Scene/Scene.h"
#include "Application/Config/Config.h"

/*
	Scene to load the ETS profile
*/

namespace fs = std::filesystem;

class MoreScene : public Scene
{
private:
	//Variables
	ImVec2 pos = ImVec2(35.0f, 35.0f);
	ImVec2 size = ImVec2(1210.0f, 650.0f);

	int mainSelectionX = 0;

	Config* config = nullptr;

	bool err = false;

	//Functions
	void Init();

	void UpdateMainSelection();

	void UpdateHelp();
	void UpdatePatchnotes();
	void UpdateCredits();

public:
	//Constructors/Destructors
	MoreScene(Application* app);
	virtual ~MoreScene();

	//Functions
	void Update();
	void Render();
	void OnLoad();
	void OnEnd();
};