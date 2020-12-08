#pragma once
#define GLEW_STATIC

#include "Scene/Scene.h"
#include "Config/Config.h"

/*
	Main Application class
*/

namespace fs = std::filesystem;

class Application 
{
private:
	//Variables
	//Window variables
	GLFWwindow* window;
	int displayW = 1280,
		displayH = 720;

	bool deleteLog = true; //If the log file should be deleted

	bool closeApp;
	ImVec4 clearColor;
	const char* glslVersion;

	Scene* currentScene;
	Scene* lastScene;
	Config config = Config();
	Utility::ResourceManager resourceManager = Utility::ResourceManager();

#ifdef _X64
	std::string version = "0.0.0561 - x64";
#else
	std::string version = "0.0.0561 - x86";
#endif // DEBUG


	//Functions
	void Init();
	void PrintSystemInfo();
	void InitWindow();
	void InitImGui();
	void InitConfig();
	void InitScene();

public:
	//Constructors/Destructors
	Application(bool deleteLog = true);
	virtual ~Application();

	//Functions
	void Run();
	void EndApp();
	void Update();
	void Render();

	//Getter/Setter
	Scene* GetScene() { return this->currentScene; }
	void SetScene(Scene* scene) { this->currentScene = scene; }

	//Window width and height
	int GetDisplayW() { return this->displayW; }
	void SetDisplayW(int w) { this->displayW = w; }
	int GetDisplayH() { return this->displayH; }
	void SetDisplayH(int H) { this->displayH = H; }

	GLFWwindow* GetWindow() { return this->window; }

	//Getter/Setter
	Config* GetConfig() { return &this->config; }

	Utility::ResourceManager* GetResourceManager() { return &this->resourceManager; }

	std::string GetAppVersion() { return this->version; }
	void SetVersion(std::string ver) { this->version = ver; }
};