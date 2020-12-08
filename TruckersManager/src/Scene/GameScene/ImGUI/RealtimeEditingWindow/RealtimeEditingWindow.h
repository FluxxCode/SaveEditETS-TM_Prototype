#pragma once

#include "Application/Config/Config.h"
#include "Utility/ResourceManager/ResourceManager.h"

class RealtimeEditingWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	Config* config = nullptr;
	Utility::ResourceManager* resourceManager = nullptr;

	DWORD	  ets2ProcessID = 0;
	uintptr_t ets2ModuleBaseAddress = 0;
	HANDLE	  ets2Process = 0;

	ImU64	  playerMoney = 0;
	uintptr_t playerMoneyAdress = 0;

	std::string supportedETSVersion = "1.37.x";

	//Functions
	void Init();
	void InitValues();

public:
	//Constructors/Destructors
	RealtimeEditingWindow();
	virtual ~RealtimeEditingWindow();

	//Functions
	void Update(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos, ImVec2 size);
};