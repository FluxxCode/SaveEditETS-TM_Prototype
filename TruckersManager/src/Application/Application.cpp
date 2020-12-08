#include "tmpch.h"
#include "Application.h"

#include "Scene/GameScene/GameScene.h"
#include "Scene/SaveEditScene/SaveEditScene.h"
#include "Scene/ConfigScene/ConfigScene.h"
#include "Scene/MoreScene/MoreScene.h"

//Functions
void Application::Init()
{
	HWND console = GetConsoleWindow();
	ShowWindow(console, 0);

	if (fs::exists("log.log") && this->deleteLog)	std::remove("log.log");

	Utility::AddLog("[Application] Version: " + this->version);
	this->PrintSystemInfo();

	this->InitWindow();
	this->InitImGui();
	this->InitConfig();

	this->InitScene();
}

void Application::PrintSystemInfo()
{
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	std::string cpuType(CPUBrandString);
	Utility::AddLog("[System] CPU: " + cpuType);


	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	Utility::AddLog("[System] Cores: " + std::to_string(sysInfo.dwNumberOfProcessors));

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	Utility::AddLog("[System] Memory: " + std::to_string((statex.ullTotalPhys / 1024) / 1024) + "MB");

	DWORD dwVersion = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwBuild = 0;

#pragma warning(suppress : 4996)
	dwVersion = GetVersion();

	//Get the Windows version
	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	//Get the build number
	if (dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));

	Utility::AddLog("[System] Windows version: " + std::to_string(dwMajorVersion) + "." + std::to_string(dwMinorVersion) + " Build: " + std::to_string(dwBuild));
}

//Creates the glfw window
void Application::InitWindow()
{
	if (!glfwInit()) //Init glfw
	{
		Utility::AddLog("[Application] ERROR: Unable to init glfw, closing app");
		this->closeApp = true;
		return;
	}

	//Init glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	this->window = glfwCreateWindow(this->displayW, this->displayH, "TruckersManager", NULL, NULL);	//Create window
	if (this->window == NULL)
	{
		Utility::AddLog("[Application] ERROR: Unable to create window, closing app");
		this->closeApp = true;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	bool err = glewInit();
	if (err != GLEW_OK)
	{
		Utility::AddLog("[Application] ERROR: Unable to init glew, closing app");
		this->closeApp = true;
		return;
	}

	//Set console size
	HWND console = GetConsoleWindow();
	RECT consoleRect;
	GetWindowRect(console, &consoleRect);
	MoveWindow(console, consoleRect.left, consoleRect.top, 1700, 700, TRUE);
}

void Application::InitImGui()
{
	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init(this->glslVersion);
}

void Application::InitConfig()
{
	if (!fs::exists("config.ini"))
	{
		Utility::AddLog("[Application] WARNING: \"config.ini\" does not exists. Creating new \"confing.ini\" with \"config_backup.ini\"");
		fs::copy("config_backup.ini", "config.ini");
	}

	if (!this->config.Load())
	{
		Utility::AddLog("[Application] WARNING: Unable to load config");
	}

	this->config.SetConfig("version", this->version);
	this->config.LoadStyle();

	if (this->config.GetConfig("console") == "true")
	{
		HWND console = GetConsoleWindow();
		ShowWindow(console, 1);
		HWND window = FindWindowA(NULL, "TruckersManager");
		SetFocus(window);
	}
}

void Application::InitScene()
{
	if (this->config.GetConfig("start_tab") == "0")
		this->currentScene = new GameScene(this);
	else if (this->config.GetConfig("start_tab") == "1")
		this->currentScene = new SaveEditScene(this);
	else if (this->config.GetConfig("start_tab") == "2")
		this->currentScene = new ConfigScene(this);
	if (this->config.GetConfig("start_tab") == "3")
		this->currentScene = new MoreScene(this);

	this->currentScene->OnLoad();
}

//Constructors/Destructors
Application::Application(bool deleteLog)
	: closeApp(false), glslVersion("#version 130"), clearColor(ImVec4(0.2f, 0.2f, 0.2f, 1.0f)), deleteLog(deleteLog)
{
	this->Init();
}

Application::~Application()
{

}

//Functions
void Application::Run()
{
	while (!glfwWindowShouldClose(this->window) && !this->closeApp)
	{
		glfwPollEvents();
		this->Update();
		this->Render();
	}
	this->EndApp();
}

void Application::EndApp()
{
	this->config.Save();

	//Cleanup
	delete this->currentScene;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Application::Update()
{
	if (this->currentScene != nullptr)
	{
		this->currentScene->Update(); //Update the current scene

		if (this->currentScene != this->lastScene)
			delete this->lastScene; //Delete last scene because the scene is changed
	}

	this->lastScene == currentScene;
}

void Application::Render()
{
	glfwGetFramebufferSize(this->window, &this->displayW, &this->displayH);
	glViewport(0, 0, this->displayW, this->displayH);
	glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, this->clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Render here */
	if (this->currentScene != nullptr)
		this->currentScene->Render(); //Render the current scene

	glfwSwapBuffers(window);
}