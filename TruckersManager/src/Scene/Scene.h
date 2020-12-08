#pragma once

#include "Application/Config/Config.h"

/*
	Base class for every scene
*/

class Application;

class Scene 
{
private:
	//Variables
	Application* app;
	Config* config;
	Utility::ResourceManager* resourceManager;

public:
	//Constructors/Destructors
	Scene(Application* app);
	virtual ~Scene();

	//Functions
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void OnLoad() = 0;
	virtual void OnEnd() = 0;

	void LoadScene(Scene* scene);

	//Getter/Setter
	Application* GetApp() { return this->app; }

	Config* GetConfig() { return this->config; }

	Utility::ResourceManager* GetResourceManager() { return this->resourceManager; }
};