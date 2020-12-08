#pragma once

#include "Scene/Scene.h"
#include "ImGUI/SelectionWindow/SelectionWindow.h"

/*
	Scene to load the ETS profile
*/

namespace fs = std::filesystem;

class GameScene : public Scene
{
private:
	//Variables
	int mainSelectionX = 0;
	SelectionWindow selectionWindow;

	//Functions
	void Init();

	void UpdateMainSelection();

public:
	//Constructors/Destructors
	GameScene(Application* app);
	virtual ~GameScene();

	//Functions
	void Update();
	void Render();
	void OnLoad();
	void OnEnd();
};